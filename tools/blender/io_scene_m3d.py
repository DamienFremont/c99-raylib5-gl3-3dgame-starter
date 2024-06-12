# ##### BEGIN MIT LICENSE BLOCK #####
#
# blender/io_scene_m3d.py
#
# Copyright (C) 2019 - 2022 bzt (bztsrc@gitlab)
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
# @brief Blender 2.80 Model 3D Exporter (and one day Importer too)
# https://gitlab.com/bztsrc/model3d
#
# ##### END MIT LICENSE BLOCK #####

# <pep8-80 compliant>

bl_info = {
    "name": "Model 3D (.m3d) format",
    "author": "bzt",
    "version": (0, 0, 1),
    "blender": (2, 80, 0),
    "location": "File > Import-Export",
    "description": "Export M3D",
    "wiki_url": "https://gitlab.com/bztsrc/model3d/blob/master/docs/m3d_format.md",
    "category": "Import-Export"}


# -----------------------------------------------------------------------------
# Import libraries
import bmesh
import os
from operator import itemgetter
from struct import pack, unpack
from mathutils import Matrix
from bpy_extras import io_utils, node_shader_utils
from bpy_extras.wm_utils.progress_report import (
    ProgressReport,
    ProgressReportSubstep,
)

# -----------------------------------------------------------------------------
#  Blender material property and M3D property type assignments
#  See https://gitlab.com/bztsrc/model3d/blob/master/docs/m3d_format.md section Materials)
mat_property_map = {
        #type  format   PrincipledBSDF property  ASCII variant
          0: ["color",  "base_color",            "Kd"],
          1: ["gscale", "metallic",              "Ka"],
          2: ["gscale", "specular",              "Ks"],
          3: ["//float","specular_tint",         "Ns"],
          4: ["//color","emissive",              "Ke"],     # not in BSDF?
          5: ["gscale", "transmission",          "Tf"],
          6: ["float",  "normalmap_strength",    "Km"],
          7: ["float",  "alpha",                 "d"],
          8: ["//byte", "illumination",          "il"],     # not in PBR at all
         64: ["float",  "roughness",             "Pr"],
         65: ["float",  "metallic",              "Pm"],
         66: ["//float","sheen",                 "Ps"],     # not in BSDF?
         67: ["float",  "ior",                   "Ni"],
        128: ["map",    "base_color_texture",    "map_Kd"],
        130: ["//map",  "specular_texture",      "map_Ks"], # should work, but it does not
        133: ["map",    "transmission_texture",  "map_Tf"],
        134: ["map",    "normalmap_texture",     "map_Km"],
        135: ["map",    "alpha_texture",         "map_D"],
        192: ["map",    "roughness_texture",     "map_Pr"],
        193: ["map",    "metallic_texture",      "map_Pm"],
        195: ["map",    "ior_texture",           "map_Ni"],
        }

# -----------------------------------------------------------------------------
# Load and parse a Model 3D file (unlike the exporter, this is WIP)
def read_m3d(context,
         filepath,
         report,
         global_matrix=None,
         ):

    # read in an index
    def getidx(data, fmt):
        idx = -1
        if fmt == 0:
            if len(data) > 0:
                idx = unpack("<B", data[0:1])[0]
                if idx > 253:
                    idx = idx - 256
                data = data[1:]
        elif fmt == 1:
            if len(data) > 1:
                idx = unpack("<H", data[0:2])[0]
                if idx > 65533:
                    idx = idx - 65536
                data = data[2:]
        elif fmt == 2:
            if len(data) > 3:
                idx = unpack("<I", data[0:4])[0]
                if idx > 4294967293:
                    idx = idx - 4294967296
                data = data[4:]
        return data, idx

    # read in a coordinate
    def getcrd(data, fmt):
        crd = 0
        if fmt == 0:
            crd = round(float(unpack("<b", data[0:1])[0]) / 127.0, 4)
            data = data[1:]
        elif fmt == 1:
            crd = round(float(unpack("<h", data[0:2])[0]) / 32767.0, 4)
            data = data[2:]
        elif fmt == 2:
            crd = unpack("<f", data[0:4])[0]
            data = data[4:]
        elif fmt == 3:
            crd = unpack("<d", data[0:8])[0]
            data = data[8:]
        if crd == -0.0:
            crd = 0.0
        return data, crd

    with ProgressReport(context.window_manager) as progress:

        ############ !!!!!!!!! Work In Progress !!!!!!!!!! ###############
        if global_matrix is None:
            global_matrix = axis_conversion(from_forward='Z', from_up='Y').to_4x4()

        # read in file
        f = open(filepath, 'rb')
        data = f.read()
        f.close()

        if len(data) > 2 and data[0:2] == b'\x1F\x8B':
            import gzip
            data = gzip.decompress(data)

        if len(data) < 8 or (data[0:4] != b'3DMO' and data[0:7] != b'3dmodel'):
            report({"ERROR"}, filepath + " is not a valid Model 3D file!")
            return {'FINISHED'}

        # load into Blender independent lists
        cmap = []
        tmap = []
        vrts = []
        faces = []
        shapes = []
        labels = []
        materials = []
        bones = []
        skins = []

        if data[0:7] == b'3dmodel':
            # Model 3D ASCII variant
            data = str(data)
            if data[0:2] == "b'":
                data = data[2:len(data)-3]  # cut off "b'" and "'"
            # parse model header
            head = {
                'name':'', 'license':'', 'author':'', 'description':'',
                'scale':1.0
            }
        else:
            # Model 3D binary variant
            # skip over preview image chunk
            if data[8:12] == b'PRVW':
                length = unpack("<I", data[4:8])[0]
                data = data[length:]

            if data[8:12] != b'HEAD':
                import zlib
                data = zlib.decompress(data[8:])
            else:
                data = data[8:]

            if data[0:4] != b'HEAD':
                report({"ERROR"}, filepath + " is not a valid Model 3D file!")
                return {'FINISHED'}

            # parse model header and string table
            head = {
                'name':'', 'license':'', 'author':'', 'description':'',
                'scale':unpack("<f", data[8:12])[0],
                'vc_s':(data[12] >> 0) & 3, 'vi_s':(data[12] >> 2) & 3, 'si_s':(data[12] >> 4) & 3,
                'ci_s':(data[12] >> 6) & 3, 'ti_s':(data[13] >> 2) & 3, 'bi_s':(data[13] >> 2) & 3,
                'nb_s':(data[13] >> 4) & 3, 'sk_s':(data[13] >> 6) & 3, 'fc_s':(data[14] >> 0) & 3,
                'hi_s':(data[14] >> 2) & 3, 'fi_s':(data[14] >> 4) & 3,
            }
            length = unpack("<I", data[4:8])[0]
            chunk = data[16:length]
            data = data[length:]
            s = chunk.split(b'\000')
            head['name'] = str(s[0], 'utf-8')
            head['license'] = str(s[1], 'utf-8')
            head['author'] = str(s[2], 'utf-8')
            head['description'] = str(s[3], 'utf-8')
            strs = {0:''}
            i = len(s[0]) + len(s[1]) + len(s[2]) + len(s[3]) + 4
            for t in s[4:]:
                strs[i] = str(t, 'utf-8')
                i = i + len(t) + 1

            # decode chunks
            while len(data) > 4 and data[0:4] != b'OMD3':
                magic = data[0:4]
                length = unpack("<I", data[4:8])[0]
                if length < 8 or length > 16 * 1024 * 1024:
                    report({"ERROR"}, str(magic,'utf-8') + " bad chunk in Model 3D file!")
                    break
                chunk = data[8:length]

                # color map
                if magic == b'CMAP':
                    while len(chunk) > 0:
                        cmap.append([
                            unpack("<B", chunk[0:1])[0] / 255.0,
                            unpack("<B", chunk[1:2])[0] / 255.0,
                            unpack("<B", chunk[2:3])[0] / 255.0,
                            unpack("<B", chunk[3:4])[0] / 255.0])
                        chunk = chunk[4:]

                # texture map
                elif magic == b'TMAP':
                    while len(chunk) > 0:
                        u = v = 0.0
                        # don't use getcrd, because this is scaled to 255
                        if head['vc_s'] == 0:
                            u = float(unpack("<B", chunk[0:1])[0]) / 255.0
                            v = float(unpack("<B", chunk[1:2])[0]) / 255.0
                            chunk = chunk[2:]
                        elif head['vc_s'] == 1:
                            u = float(unpack("<H", chunk[0:2])[0]) / 65535.0
                            v = float(unpack("<H", chunk[2:4])[0]) / 65535.0
                            chunk = chunk[4:]
                        elif head['vc_s'] == 2:
                            u, v = unpack("<ff", chunk[0:8])
                            chunk = chunk[8:]
                        elif head['vc_s'] == 3:
                            u, v = unpack("<dd", chunk[0:16])
                            chunk = chunk[16:]
                        tmap.append([round(u, 4), round(v, 4)])

                # vertex list (for vertex and normals)
                elif magic == b'VRTS':
                    while len(chunk) > 0:
                        chunk, x = getcrd(chunk, head['vc_s'])
                        chunk, y = getcrd(chunk, head['vc_s'])
                        chunk, z = getcrd(chunk, head['vc_s'])
                        chunk, w = getcrd(chunk, head['vc_s'])
                        chunk, c = getidx(chunk, head['ci_s'])
                        chunk, s = getidx(chunk, head['sk_s'])
                        if ci_s < 4 and c != -1:
                            c = cmap[c]
                        elif ci_s == 4:
                            c = [c[0:1] / 255.0, c[1:2] / 255.0, c[2:3] / 255.0, c[3:4] / 255.0]
                        vrts.append([x, y, z, w, c, s])

                # material chunk
                elif magic == b'MTRL':
                    chunk, n = getidx(chunk, head['si_s'])
                    props = {}
                    while len(chunk) > 0:
                        typ = unpack("<B", chunk[0:1])[0]
                        chunk = chunk[1:]
                        try:
                            t = mat_property_map[typ];
                        except:
                            print("unknown property ", typ)
                            break;
                        if typ == 0:
                            chunk, c = getidx(chunk, head['ci_s'])
                            props["base_color"] = cmap[c]
                            props["alpha"] = cmap[c][3]
                        elif typ >= 128:
                            chunk, s = getidx(chunk, head['si_s'])
                            if t[1][0:2] != "//":
                                props[t[1]] = strs[s] + ".png"
                        elif t[0] == "gscale":
                            chunk, c = getidx(chunk, head['ci_s'])
                            props[t[1]] = round((cmap[c][0] + cmap[c][1] + cmap[c][2]) / 3, 4)
                        elif t[0] == "color":
                            chunk, c = getidx(chunk, head['ci_s'])
                            if ci_s < 4 and c != -1:
                                c = cmap[c]
                            elif ci_s == 4:
                                c = [c[0:1] / 255.0, c[1:2] / 255.0, c[2:3] / 255.0, c[3:4] / 255.0]
                            props[t[1]] = c
                        elif t[0] == "float":
                            chunk, c = getcrd(chunk, 2)
                            props[t[1]] = c
                        elif t[0] == "byte":
                            props[t[1]] = unpack("<B", chunk[0:1])[0]
                            chunk = chunk[1:]
                        elif t[0] == "int":
                            props[t[1]] = unpack("<I", chunk[0:4])[0]
                            chunk = chunk[4:]
                        elif t[0] == "//map":
                            chunk = chunk[head['si_s']:]
                        elif t[0] == "//color" or t[0] == "//gscale":
                            chunk = chunk[head['ci_s']:]
                        elif t[0] == "//float" or t[0] == "//int":
                            chunk = chunk[4:]
                        elif t[0] == "//byte":
                            chunk = chunk[1:]
                        else:
                            break;
                    materials.append([strs[n], props])

                # mesh (only triangles supported for now)
                elif magic == b'MESH':
                    m = -1
                    while len(chunk) > 0:
                        typ = unpack("<B", chunk[0:1])[0]
                        chunk = chunk[1:]
                        if typ == 0:
                            chunk, s = getidx(chunk, head['si_s'])
                            m = -1
                            if s != 0:
                                for i, mat in enumerate(materials):
                                    if strs[s] == mat[0]:
                                        m = i
                                        break
                        elif (typ >> 4) == 3:
                            v = [-1, -1, -1]
                            t = [-1, -1, -1]
                            n = [-1, -1, -1]
                            for i in range(0, 3):
                                chunk, v[i] = getidx(chunk, head['vi_s'])
                                if typ & 1:
                                    chunk, t[i] = getidx(chunk, head['ti_s'])
                                if typ & 2:
                                    chunk, n[i] = getidx(chunk, head['vi_s'])
                            faces.append({'m':m, 'v':v, 't':t, 'n':n})
                        else:
                            print("Only triangles supported")
                            break

                # shapes
                elif magic == b'SHPE':
                    chunk, s = getidx(chunk, head['si_s'])
                    print("shape ", strs[s])

                # annotation labels
                elif magic == b'LBLS':
                    chunk, n = getidx(chunk, head['si_s'])
                    chunk, l = getidx(chunk, head['si_s'])
                    chunk, c = getidx(chunk, head['ci_s'])
                    if ci_s < 4 and c != -1:
                        c = cmap[c]
                    elif ci_s == 4:
                        c = [c[0:1] / 255.0, c[1:2] / 255.0, c[2:3] / 255.0, c[3:4] / 255.0]
                    print("Labels layer:", strs[n], ", lang:", strs[l], ", color ", c)

                # armature bones and vertex groups
                elif magic == b'BONE':
                    chunk, b = getidx(chunk, head['bi_s'])
                    chunk, s = getidx(chunk, head['sk_s'])
                    print("Skeleton ", b, "bone(s),", s, "skin record(s) (unique bone/weight combos)")

                # animation and timeline marker
                elif magic == b'ACTN':
                    chunk, s = getidx(chunk, head['si_s'])
                    chunk, f = getidx(chunk, 1)
                    chunk, l = getidx(chunk, 2)
                    print("Action ", strs[s], ", durationmsec", l, ", numframes ", f)

                # inlined asset
                elif magic == b'ASET':
                    chunk, s = getidx(chunk, head['si_s'])
                    print("Inlined asset ", strs[s], "(", len(chunk), " bytes)")

                else:
                    print("Unknown chunk '%s' skipping..." % (str(magic,'utf-8')))

                data = data[length:]
            del strs
            del cmap

        # ----------------- Start of Blender Specific Stuff ---------------------
        print("\nhead ", head)
        print("\ntmap ", tmap)            # texture map, array of [u, v]
        print("\nvrts ", vrts)            # vertex list, array of [x, y, z, w, [r,g,b,a], skinid]
        print("\nmaterials", materials)   # array of [name, array of [principledBSDFpropname, value]]
        print("\nfaces ", faces)          # triangles, array of [m:materialidx, v[3]:vertexidx, t[3]:tmapidx, n[3]:normalvertexidx]
        print("\nshapes ", shapes)
        print("\nlabels ", labels)
        print("\nbones ", bones)
        print("\nskins ", skins)
        # ----------------- End of Blender Specific Stuff ---------------------

        report({"ERROR"}, "Model 3D importer not fully implemented yet.")

    return {'FINISHED'}


# -----------------------------------------------------------------------------
# Construct and save a Model 3D file
def write_m3d(context,
         filepath,
         report,
         *,
         use_name='',                   # model's name
         use_license='MIT',             # model's license
         use_author='',                 # model's author
         use_comment='',                # model's comment
         use_scale=1.0,                 # model-space 1.0 in SI meters
         use_selection=True,            # export selected items only
         use_mesh_modifiers=True,       # apply mesh modifiers
         use_normals=False,             # save normal vectors too
         use_uvs=True,                  # save texture map UV coordinates
         use_colors=True,               # save per vertex colors
         use_shapes=False,              # save shape commands
         use_materials=True,            # save materials
         use_skeleton=True,             # save bind-pose armature
         use_animation=True,            # save skeletal animations
         use_markers=False,             # use timeline markers for animations
         use_fps=25,                    # frame per second
         use_quality='-1',              # -1: auto, 0: 8 bit, 1: 16 bit, 2: 32 bit, 3: 64 bit
         use_inline=False,              # inline textures
         use_gridcompress=True,         # use lossy grid compression
         use_strmcompress=True,         # use lossless stream compression
         use_ascii=False,               # save ASCII variant
         use_relbones=True,             # (debug only) use parent relative bone positions
         global_matrix=None,            # default orientation
         check_existing=True,
         ):

    # convert string to name identifier
    def safestr(name, morelines=0):
        if name is None:
            return ''
        elif morelines == 3:
            return name.replace('\r', '').strip()
        elif morelines == 2:
            return name.replace('\r', '').replace('\n', ' ').strip()
        elif morelines == 1:
            return name.replace('\r', '').replace('\n', '\r\n').strip()
        else:
            return name.replace(' ', '_').replace('/', '_').replace('\\', '_').replace('\r', '').replace('\n', ' ').strip()

    # set is unique, but has no index, list has index, but not unique...
    # this is utterly and painfully slow, hence the dict wrapper below
    def uniquelist(l, e):
        try:
            i = l.index(e)
        except ValueError:
            i = len(l)
            l.append(e)
        return i

    # use hash table and then convert dict to list instead
    # this uses considerably more memory, but we have no choice:
    # using uniquelist takes several minutes with 50000 triangles...
    def uniquedict(l, e):
        h = hash(str(e))
        try:
            return l[h][0]
        except KeyError:
            i = len(l)
            l[h] = [i, e]
        return i

    def dict2list(l):
        r = []
        for i, v in l.items():
            r.insert(v[0], v[1])
        return r

    # get index size (we use -1 and -2 as special indices)
    def idxsize(cnt):
        if cnt == 0:
            return 3
        elif cnt < 254:
            return 0
        elif cnt < 65534:
            return 1
        return 2

    # write out an index
    def addidx(fmt, idx):
        # we rely on the fact that in C -1 is a full binary 1 which
        # gives the maximum unsigned value regardless to size, but
        # pack stops us from taking advantage of that
        if fmt == 0:
            if idx < 0:
                idx = 256 + idx
            return pack("<B", idx)
        elif fmt == 1:
            if idx < 0:
                idx = 65536 + idx
            return pack("<H", idx)
        elif fmt == 2:
            if idx < 0:
                idx = 4294967296 + idx
            return pack("<I", idx)
        return b''

    # eliminate minus zero
    def vert(x,y,z,w,c,s):
        if x == -0.0:
            x = 0.0
        if y == -0.0:
            y = 0.0
        if z == -0.0:
            z = 0.0
        if w == -0.0:
            w = 0.0
        return [x,y,z,w,c,s]

    # normalize matrix, decompose and recompose to eliminate errors
    def matnorm(a):
        p, q, s = a.decompose()
        q.normalize()
        return Matrix.Translation(p) @ q.to_matrix().to_4x4()

    # get texture
    def gettexture(fn, use_inline):
        data = b''
        if fn[0:2] == "//":
            fn = fn[2:]
        imgpath = repr(os.path.basename(fn))[1:-1]
        imgpath = os.path.splitext(imgpath)[0]
        if imgpath != "" and use_inline:
            try:
                data = open(os.path.join(os.path.dirname(filepath), fn), 'rb').read()
            except:
                try:
                    data = open(os.path.join(os.path.dirname(filepath), os.path.basename(fn)), 'rb').read()
                except:
                    try:
                        data = open(os.path.join(os.path.dirname(filepath), imgpath + ".png"), 'rb').read()
                    except:
                        try:
                            data = open(imgpath + ".png", 'rb').read()
                        except:
                            try:
                                data = open(fn, 'rb').read()
                            except:
                                data = b''
            if len(data) < 8 or data[0:4] != b'\x89PNG':
                report({"ERROR"}, "Texture file '" + fn + "' not found or not a valid PNG. Cannot be inlined.")
                data = b''
        return [ imgpath, data ]

    # recursively walk skeleton and construct string representation
    def bonestr(strs, bones, parent, level):
        ret = ""
        for i,b in enumerate(bones):
            if b[0] == parent:
                ret += "/"*level + str(b[2]) + " " + str(b[3]) + " " + strs[b[1]] + "\r\n"
                ret += bonestr(strs, bones, i, level+1)
        return ret

    with ProgressReport(context.window_manager) as progress:

        if global_matrix is None:
            global_matrix = axis_conversion(from_forward='-Y', from_up='Z',to_forward='Z', to_up='Y').to_4x4()
        if use_animation:
            use_skeleton = True
        if use_fps < 1 or use_fps > 120:
            use_fps = 25

        # Get Blender objects to export
        depsgraph = context.evaluated_depsgraph_get()
        scene = context.scene
        if use_selection:
            objects = context.selected_objects
        else:
            objects = context.scene.objects

        # if use_quality is set to auto, then count the number of triangles to decide
        use_quality = int(use_quality)
        if use_quality < 0 or use_quality > 3:
            n = 0
            for i, ob_main in enumerate(objects):
                if ob_main.parent and ob_main.parent.instance_type in {'VERTS', 'FACES'}:
                    continue
                try:
                    me = ob_main.original.to_mesh()
                    n += len(me.polygons)
                except:
                    continue
            if n < 1024:
                use_quality = 0
            else:
                use_quality = 1
        # we must use floating point without grid compression
        if use_gridcompress == False and use_quality < 2:
            use_quality = 2
        # get the number of significant digits depending on quality
        if use_quality == 3:
            digits = 15
        if use_quality == 2:
            digits = 7
        else:
            digits = 4

        # Build global lists with unique elements
        # we use a dict wrapper to speed up things
        cmap = {}       # color map entries
        strs = {}       # string table with unique strings
        verts = {}      # unique list of vertices
        tmaps = {}      # texture map UV coordinates
        faces = []      # triangles list
        shapes = []     # shapes list
        labels = []     # annotation labels
        materials = []  # translated material name and properties
        bones = {}      # bind-pose skeleton
        skins = {}      # array of bone id / weight combinations per vertex
        actions = []    # animations
        inlined = {}    # inlined textures
        extras = []     # extra chunks (engine specific)
        progress.enter_substeps(2 + use_materials + use_skeleton + use_animation)

        # ----------------- Start of Blender Specific Stuff ---------------------
        refmats = {}    # unique list of referenced Blender material objects
        nb_m = 0        # maximum number of bone weights per vertex
        fi_m = 0        # frame index maximum

        # set rest armature (bind-pose skeleton)
        # if we don't do this, we'll get strange bones and distorted mesh
        oldaction = None
        oldframe = context.scene.frame_current
        oldpose = {}
        for i,ob_main in enumerate(objects):
            if ob_main.type == "ARMATURE":
                oldpose[i] = ob_main.data.pose_position
                ob_main.data.pose_position = "REST"
                ob_main.data.update_tag()
                if oldaction == None and ob_main.animation_data and ob_main.animation_data.action:
                    oldaction = ob_main.animation_data.action
        context.scene.frame_set(0)

        ### Armature ###
        if use_skeleton:
            # this must be done before the mesh so that skin can refer to bones
            progress.step("Exporting Armature")
            idx = 0
            for i,ob_main in enumerate(objects):
                if ob_main.type != "ARMATURE":
                    continue
                for b in ob_main.data.bones:
                    m = matnorm(global_matrix @ ob_main.matrix_world @ b.matrix_local)
                    a = -1
                    if b.parent:
                        # is there a better way to get the parent's
                        # index in the armature's bone collection?
                        for j,p in enumerate(ob_main.data.bones):
                            if p == b.parent:
                                a = j
                                break
                        if use_relbones == True:
                            p = matnorm(global_matrix @ ob_main.matrix_world @ b.parent.matrix_local)
                            m = p.inverted() @ m
                    # For the top level bones, we need model-space p,q
                    # for the children, parent relative p,q
                    p = m.to_translation()  # position
                    q = m.to_quaternion()   # orientation
                    q.normalize()
                    n = safestr(b.name)
                    try:
                        ni = strs[hash(str(n))][0]
                        name = "'" + b.name + "'"
                        if b.name != n:
                            name += " (" + n + ")"
                        report({"ERROR"}, "Bone name " + name + " not unique.")
                        use_skeleton = False
                        use_animation = False
                        bones = {}
                        break
                    except:
                        pass
                    bones[b.name] = [idx, [a, uniquedict(strs, n),
                        uniquedict(verts, vert(
                            round(p[0], digits),
                            round(p[1], digits),
                            round(p[2], digits), 1.0, 0, -1)),
                        uniquedict(verts, vert(
                            round(q.x, digits),
                            round(q.y, digits),
                            round(q.z, digits),
                            round(q.w, digits), 0, -2))]]
                    idx = idx + 1
            if len(bones) < 1 and use_animation:
                report({"ERROR"}, "Skipping skeletal animation in lack of armature.")
                use_animation = False

        ### Mesh data ###
        progress.step("Exporting Mesh")
        progress.enter_substeps(len(objects))
        for i, ob_main in enumerate(objects):
            # this mess was taken from io_scene_obj. The point is, at the end we have
            # something that has faces with triangles and model-space coordinate vertices
            if ob_main.parent and ob_main.parent.instance_type in {'VERTS', 'FACES'}:
                continue

            obs = [(ob_main, ob_main.matrix_world)]
            if ob_main.is_instancer:
                obs += [(dup.instance_object.original, dup.matrix_world.copy())
                        for dup in depsgraph.object_instances
                        if dup.parent and dup.parent.original == ob_main]
            for ob, ob_mat in obs:
                # get a copy of the mesh object
                try:
                    o = ob.evaluated_get(depsgraph) if use_mesh_modifiers else ob.original
                    me = o.to_mesh()
                except:
                    me = None
                if me is None or len(me.polygons) < 1:
                    continue
                if use_name is None or use_name == '':
                    use_name = ob.name

                # triangulate if we must
                r = False
                for poly in me.polygons:
                    if len(poly.loop_indices) != 3:
                        r = True
                        break
                if r == True:
                    #print("Need to triangulate mesh '" + me.name + "'")
                    bm = bmesh.new()
                    bm.from_mesh(me)
                    bmesh.ops.triangulate(bm, faces=bm.faces[:])
                    bm.to_mesh(me)
                    bm.free()

                # transform vertices to model-space
                me.transform(global_matrix @ ob_mat)
                if ob_mat.determinant() < 0.0:
                    me.flip_normals()
                if use_normals:
                    me.calc_normals_split()

                if use_skeleton and len(ob.vertex_groups) > 0:
                    vg = ob.vertex_groups
                else:
                    vg = []
                    if use_skeleton == True and use_animation == True:
                        report({"ERROR"}, "Mesh '" + me.name + "' in object '" + ob.name + "' has no vertex groups, no skeletal animation possible!")

                if use_uvs and len(me.uv_layers) > 0:
                    uv_layer = me.uv_layers.active.data[:]
                else:
                    uv_layer = []

                if use_colors and len(me.vertex_colors) > 0:
                    # bug in Blender 3.6, vertex_colors.active_index might be out of bounds...
                    if me.vertex_colors.active_index >= 0 and me.vertex_colors.active_index < len(me.vertex_colors) and len(me.vertex_colors[me.vertex_colors.active_index].data) > 0:
                        vcol = me.vertex_colors[me.vertex_colors.active_index].data
                    else:
                        report({"ERROR"}, "Vertex color in mesh '" + me.name + "' in object '" + ob.name + "' has invalid out-of-bounds index (vertex_colors.active_index is " + str(me.vertex_colors.active_index) + ", largest can be " + str(len(me.vertex_colors) - 1) + ").")
                        # try to fallback to the first vertex_colors index
                        if len(me.vertex_colors[0].data) > 0:
                            vcol = me.vertex_colors[0].data
                        else:
                            report({"ERROR"}, "Vertex color in mesh '" + me.name + "' in object '" + ob.name + "' unable to fallback to vertex_colors[0], no data.")
                            vcol = []
                else:
                    vcol = []

                matnames = []
                if use_materials:
                    for m in me.materials[:]:
                        if m and m.name:
                            matnames.append(uniquedict(strs, safestr(m.name)))
                        else:
                            matnames.append(-1)

                # Ahhh finally we can get the vertices and faces
                badref = {}
                for pi,poly in enumerate(me.polygons):
                    face = [ -1, [-1,-1,-1], [-1,-1,-1], [-1,-1,-1], -1 ]
                    if len(matnames) > 0:
                        if poly.material_index < len(matnames):
                            i = poly.material_index
                        else:
                            i = 0
                            # workaround to report each bad material index only once
                            try:
                                dummy = badref[poly.material_index]
                            except:
                                badref[poly.material_index] = 1
                                report({"ERROR"}, "Polygon face in mesh '" + me.name + "' referencing a non-existent material (index " + str(poly.material_index) + ", largest can be " + str(len(matnames) - 1) + ").")
                        if i >= 0:
                            face[0] = matnames[i]
                            uniquedict(refmats, me.materials[i])
                    for i, li in enumerate(poly.loop_indices):
                        if len(vcol) > 0:
                            c = uniquedict(cmap, [vcol[li].color[0], vcol[li].color[1], vcol[li].color[2], vcol[li].color[3]])
                        else:
                            c = 0
                        v = me.vertices[poly.vertices[i]]
                        if use_skeleton and len(vg) > 0 and len(v.groups) > 0:
                            wf = 0.0
                            for g in v.groups:
                                wf += g.weight
                            if wf > 0.0:
                                skin = []
                                w = wi = wm = 0
                                for g in v.groups:
                                    try:
                                        s = round(g.weight / wf * 255.0)
                                        if s > wm:
                                            wm = s
                                            si = len(skin)
                                        if s < 1:
                                            s = 1
                                        if s > 255:
                                            s = 255
                                        skin.append([bones[vg[g.group].name][0], s])
                                        w = w + s
                                    except:
                                        report({"ERROR"}, "Vertex group name '" + vg[g.group].name + "' does not match any bone.")
                                        use_skeleton = False
                                        vg = []
                                        s = -1
                                        break
                                try:
                                    if w != 255:
                                        skin[si][1] += 255 - w
                                except:
                                    pass
                                s = uniquedict(skins, skin)
                                if len(skin) > nb_m:
                                    nb_m = len(skin)
                            else:
                                s = -1
                        else:
                            s = -1
                        face[1][i] = uniquedict(verts, vert(
                            round(v.co.x, digits),
                            round(v.co.y, digits),
                            round(v.co.z, digits), 1.0, c, s))
                        if use_normals:
                            try:
                                no = v.normal.copy()
                            except:
                                no = poly.loops[i].normal.copy()
                            no.normalize()
                            face[3][i] = uniquedict(verts, vert(
                                round(no.x, digits),
                                round(no.y, digits),
                                round(no.z, digits), 1.0, 0, -1))
                            del no
                        if use_uvs and len(uv_layer) > 0:
                            face[2][i] = uniquedict(tmaps, list(uv_layer[li].uv[:]))
                    faces.append(face)
                del me
            progress.step()
        progress.leave_substeps()

        ### Materials ###
        if use_materials:
            progress.step("Exporting Materials")
            progress.enter_substeps(len(refmats))
            matopa = {}
            matopa[-1] = -1
            for i,v in refmats.items():
                mi = v[0]
                mat = v[1]
                if mat is not None:
                    props = {}
                    d = 1.0
                    if mat.node_tree:
                        # at least try to get the diffuse texture from other material types,
                        # because not all wrapped in PrincipledBSDF properly
                        for n in mat.node_tree.nodes:
                            if n.type == 'TEX_IMAGE' and n.image and n.image.filepath and n.image.filepath != "" and n.image.filepath != "//":
                                imgpath, data = gettexture(n.image.filepath, use_inline)
                                if imgpath != "":
                                    s = uniquedict(strs, imgpath)
                                    if use_inline and len(data) > 8:
                                        uniquedict(inlined, [s, data])
                                    props[128] = [128, s]
                                break
                    # otherwise properly parse material if blender can convert it into PrincipledBSDF
                    mat_wrap = node_shader_utils.PrincipledBSDFWrapper(mat)
                    if mat_wrap:
                        for key, mat_wrap_key in mat_property_map.items():
                            if key == 0:
                                # Kd
                                if mat_wrap.alpha != 0.0 and mat_wrap.alpha != 1.0:
                                    d = mat_wrap.alpha
                                elif mat_wrap.base_color and len(mat_wrap.base_color) > 3:
                                    d = mat_wrap.base_color[3]
                                else:
                                    d = 0.0
                                if d != 0.0:
                                    props[0] = [0, uniquedict(cmap, [mat_wrap.base_color[0], mat_wrap.base_color[1], mat_wrap.base_color[2], d])]
                            elif key == 8:
                                # il
                                il = 0
                                if mat_wrap.specular == 0:
                                    il = 1
                                elif mat_wrap.metallic != 0.0:
                                    if d != 1.0:
                                        il = 6
                                    else:
                                        il = 3
                                elif d != 1.0:
                                    il = 9
                                else:
                                    il = 2
                                if il != 0:
                                    props[8] = [8, il]
                            elif mat_wrap_key[0][0:2] == "//":
                                continue

                            try:
                                val = getattr(mat_wrap, mat_wrap_key[1], None)
                            except:
                                continue
                            if val is None:
                                continue

                            if key >= 128:
                                # according to the doc, texture material attributes should always have val.image
                                # but sometimes they don't... And sometimes filename is "//" for whatever reason...
                                if val.image is None or val.image.filepath is None or val.image.filepath == "" or val.image.filepath == "//":
                                    continue
                                imgpath, data = gettexture(val.image.filepath, use_inline)
                                if imgpath == "":
                                    continue
                                s = uniquedict(strs, imgpath)
                                props[key] = [key, s]
                                if use_inline and len(data) > 8:
                                    uniquedict(inlined, [s, data])
                            elif mat_wrap_key[0] == "gscale" and val != 0.0:
                                props[key] = [key, uniquedict(cmap, [val, val, val, 1.0])]
                            elif mat_wrap_key[0] == "color" and len(val) == 3:
                                props[key] = [key, uniquedict(cmap, [val[0], val[1], val[2], 1.0])]
                            elif mat_wrap_key[0] == "color" and len(val) == 4:
                                props[key] = [key, uniquedict(cmap, val)]
                            elif mat_wrap_key[0] == "float" and val != 0.0:
                                props[key] = [key, val]
                            elif (mat_wrap_key[0] == "byte" or mat_wrap_key[0] == "int") and val != 0:
                                props[key] = [key, val]
                    else:
                        report({"ERROR"}, "Material '" + mat.name + "' does not use PrincipledBSDF surface, not parsing.")
                    # append material if it has at least one property
                    if len(props) > 0:
                        ni = uniquedict(strs, safestr(mat.name))
                        matopa[ni] = 255 - int(255.0 * d)
                        materials.append([ni, props])
                progress.step()
            progress.leave_substeps()
            # sort faces by opacity and material index
            for i,v in enumerate(faces):
                try:
                    faces[i][4] = matopa[faces[i][0]]
                except:
                    faces[i][4] = 255
            faces.sort(key=itemgetter(4,0))
        else:
            # sort faces by material index only
            faces.sort(key=itemgetter(0))

        ### Actions ###
        if use_animation:
            progress.step("Exporting Animations")
            if use_skeleton and len(bones) > 0:
                mpf = 1000.0/use_fps # msec per frame
                acts = []
                nf = 0 # number of total frames
                # collect actions from timeline markers, otherwise use actions
                if use_markers == True:
                    if len(scene.timeline_markers) > 0:
                        tlm = sorted(scene.timeline_markers, key=lambda tl: tl.frame)
                        for i,t in enumerate(tlm):
                            if i + 1 >= len(tlm):
                                et = scene.frame_end
                            else:
                                et = tlm[i+1].frame - 1
                            if et > t.frame:
                                acts.append([safestr(t.name), -1, t.frame, et])
                                nf = nf + et - t.frame
                        del tlm
                else:
                    for i,a in enumerate(bpy.data.actions):
                        # pre 3.0 blender used frame_range for the animation's length
                        # but post 3.0 blender uses that for a manually selected range
                        st = et = 0
                        try:
                            st = int(a.curve_frame_range[0])
                            et = int(a.curve_frame_range[1])
                        except:
                            st = int(a.frame_range[0])
                            et = int(a.frame_range[1])
                        if et > 0:
                            acts.append([safestr(a.name), i, st, et])
                        nf += et - st
                if nf == 0:
                    # no actions nor markers, one big happy animation only
                    acts.append(["Anim", -1, scene.frame_start, scene.frame_end])
                    nf = scene.frame_end - scene.frame_start
                # ok, now 'acts' is an array of [action name, action pose index, start frame, end frame]
                progress.enter_substeps(nf + 1)
                for a in acts:
                    # set action pose
                    scene.frame_set(0, subframe=0.0)
                    for i,ob_main in enumerate(objects):
                        if ob_main.type != "ARMATURE":
                            continue
                        if a[1] != -1:
                            ob_main.animation_data.action = bpy.data.actions[a[1]]
                        ob_main.data.pose_position = "POSE"
                        ob_main.data.update_tag()
                    lf = 0
                    frames = []     # collect frame with changed bones for this action
                    lastpose = {}   # fill up with bind pose on start
                    for n,b in bones.items():
                        lastpose[n] = [b[1][2], b[1][3]]
                    # iterate through each frame, and set anim pose for the armature
                    for frame in range(a[2], a[3] + 1):
                        scene.frame_set(frame, subframe=0.0)
                        # walk through the bones in anim pose, collect which one changed
                        changed = []
                        for i,ob_main in enumerate(objects):
                            if ob_main.type != "ARMATURE":
                                continue
                            for i, b in enumerate(ob_main.pose.bones):
                                try:
                                    idx = bones[b.name][0]
                                except:
                                    report({"ERROR"}, "Animated bone name '" + b.name + "' does not match any bind-pose bone???")
                                    break;
                                # we need model-space p,q only for bones without parents
                                m = matnorm(global_matrix @ ob_main.matrix_world @ b.matrix)
                                if use_relbones == True and b.parent:
                                    p = matnorm(global_matrix @ ob_main.matrix_world @ b.parent.matrix)
                                    m = p.inverted() @ m
                                p = m.to_translation()
                                q = m.to_quaternion()
                                q.normalize()
                                # differerent?
                                pos = uniquedict(verts, vert(
                                        round(p[0], digits),
                                        round(p[1], digits),
                                        round(p[2], digits), 1.0, 0, -1))
                                ori = uniquedict(verts, vert(
                                        round(q.x, digits),
                                        round(q.y, digits),
                                        round(q.z, digits),
                                        round(q.w, digits), 0, -2))
                                if lastpose[b.name][0] != pos or lastpose[b.name][1] != ori:
                                    changed.append([idx, pos, ori])
                                    lastpose[b.name][0] = pos
                                    lastpose[b.name][1] = ori
                        # do we have changed bones on this frame?
                        if len(changed) > 0:
                            if len(frames) < 1:
                                a[2] = frame
                            frames.append([int((frame-a[2]) * mpf), changed])
                            lf = frame
                            if len(changed) > fi_m:
                                fi_m = len(changed)
                        progress.step()
                    # if the action has at least one frame, save it
                    if len(frames) > 0:
                        actions.append([uniquedict(strs, safestr(a[0])), int((lf-a[2]+1) * mpf), frames])
                progress.leave_substeps()
            else:
                report({"ERROR"}, "Trying to export animations without armature and skin")
        # restore original armature
        for i,ob_main in enumerate(objects):
            if ob_main.type == "ARMATURE":
                if oldaction != None and ob_main.animation_data:
                    try:
                        ob_main.animation_data.action = oldaction
                    except:
                        continue
                ob_main.data.pose_position = oldpose[i]
                ob_main.data.update_tag()
        context.scene.frame_set(oldframe)

        # we need lists, but creating unique lists in python is impossible, so we
        # have used dictionaries. Let's convert those into lists now
        cmap = dict2list(cmap)
        strs = dict2list(strs)
        verts = dict2list(verts)
        tmaps = dict2list(tmaps)
        bones = dict2list(bones)
        skins = dict2list(skins)
        inlined = dict2list(inlined)
        # ----------------- End of Blender Specific Stuff ---------------------

        # Now we should have:
        #  cmap = array of [r, g, b, a]
        #  strs = array of unique strings
        #  verts = array of [x, y, z, w, color, skinid]
        #  tmaps = array of [u, v]
        #  faces = array of [material strid, [3] vertexids, [3] normalvertexids, [3] tmapids }
        #  shapes =
        #  labels =
        #  materials = array of [material strid, dict of [property type, property value]]
        #  bones = array of [parent, name strid, pos vertexid, ori vertexid]
        #  skins = array of [[boneid, weight] * 8]
        #  actions = array of [action name strid, durationmsec, array of animation frames]
        #    anim frame = [timestampmsec, array of [boneid, pos vertexid, ori vertexid]]
        #  inlined = array of [name strid, bytes data]
        #  extras = array of [bytes[4] magic, bytes data]

        #print("----------------------------------------------")
        #print(cmap)
        #print(strs)
        #print(verts)
        #print(tmaps)
        #print(faces)
        #print(shapes)
        #print(labels)
        #print(materials)
        #print(bones)
        #print(skins)
        #print(actions)
        #print(inlined)
        #print(extras)
        #print("----------------------------------------------")

        # normalize coordinates
        if use_gridcompress == True:
            min_x = min_y = min_z = 1e10
            max_x = max_y = max_z = -1e10
            for v in verts:
                if v[0] < min_x:
                    min_x = v[0]
                if v[0] > max_x:
                    max_x = v[0]
                if v[1] < min_y:
                    min_y = v[1]
                if v[1] > max_y:
                    max_y = v[1]
                if v[2] < min_z:
                    min_z = v[2]
                if v[2] > max_z:
                    max_z = v[2]
            s = max(abs(min_x), abs(max_x), abs(min_y), abs(max_y), abs(min_z), abs(max_z))
            if s != 1.0 and s != 0.0:
                for i,v in enumerate(verts):
                    if verts[i][5] != -2:
                        verts[i][0] = round(verts[i][0] / s, digits)
                        verts[i][1] = round(verts[i][1] / s, digits)
                        verts[i][2] = round(verts[i][2] / s, digits)
            if use_scale <= 0.0:
                use_scale = s
        if use_scale <= 0.0:
            use_scale = 1.0

        # Construct chunks buffer from lists
        progress.step("Compressing output")
        print(len(verts), "verts,", len(faces), "faces,", len(tmaps), "UVs", len(materials), "materials,", len(bones), "bones,", len(skins), "skins,", len(actions), "actions")

        # create string table and calculate string offsets
        if use_author is None or use_author == "":
            use_author = os.getenv("LOGNAME", "")

        if use_ascii == True:
            # save Model 3D ASCII variant
            s = "3dmodel " + str(use_scale) + "\r\n"
            s += safestr(use_name, 2) + "\r\n"
            s += safestr(use_license, 2) + "\r\n"
            s += safestr(use_author, 2) + "\r\n"
            s += safestr(use_comment, 1) + "\r\n\r\n"

            # materials
            if len(materials) > 0:
                for m in materials:
                    s += "Material " + strs[m[0]] + "\r\n"
                    for pi,p in m[1].items():
                        t = mat_property_map[p[0]]
                        s += t[2] + " "
                        if t[0] == "color" or t[0] == "gscale":
                            s += "#"
                            for i in range(0, 4):
                                s += "%02x" % (int(cmap[p[1]][3 - i] * 255.0))
                        elif t[0] == "float":
                            s += str(round(p[1], digits))
                        elif p[0] >= 128:
                            s += strs[p[1]]
                        else:
                            s += str(p[1])
                        s += "\r\n"
                    s += "\r\n"

            # texture map
            if len(tmaps) > 0:
                s += "Textmap\r\n"
                r = True
                for t in tmaps:
                    # failsafes
                    if t[0] < 0.0 or t[0] > 1.0 or t[1] < 0.0 or t[1] > 1.0:
                        if r:
                            r = False
                            report({"ERROR"}, "Texture UV's are out of 0..1 range")
                        if t[0] > 1.0:
                            t[0] = 1.0
                        if t[0] < 0.0:
                            t[0] = 0.0
                        if t[1] > 1.0:
                            t[1] = 1.0
                        if t[1] < 0.0:
                            t[1] = 0.0
                    s += str(round(t[0], digits)) + " " + str(round(t[1], digits)) + "\r\n"
                s += "\r\n"

            # vertex list
            if len(verts) > 0:
                s += "Vertex\r\n"
                for v in verts:
                    s += str(v[0]) + " " + str(v[1]) + " " + str(v[2]) + " " + str(v[3])
                    if v[4] >= 0 and v[4] < len(cmap):
                        s += " #"
                        for i in range(0, 4):
                            s += "%02x" % (int(cmap[v[4]][3 - i] * 255.0))
                    elif v[5] >= 0 and v[5] < len(skins):
                        s += " #ffffffff"
                    if v[5] >= 0 and v[5] < len(skins):
                        for i in range(0, min(len(skins[v[5]]), 8)):
                            if skins[v[5]][i][0] != -1 and skins[v[5]][i][1] != 0:
                                s += " " + str(skins[v[5]][i][0]) + ":" + str(round(float(skins[v[5]][i][1]) / 255.0, 4))
                    s += "\r\n"
                s += "\r\n"

            # triangle mesh
            if len(faces) > 0:
                s += "Mesh\r\n"
                l = -1
                for f in faces:
                    if l != f[0]:
                        l = f[0]
                        if l == -1:
                            s += "use\r\n"
                        else:
                            s += "use " + strs[l] + "\r\n"
                    for i,v in enumerate(f[1]):
                        if i != 0:
                            s += " "
                        s += str(v) + "/"
                        if use_uvs:
                            s += str(f[2][i])
                        s += "/"
                        if use_normals:
                            s += str(f[3][i])
                    s += "\r\n"
                s += "\r\n"

            # skeleton
            if len(bones) > 0 or len(skins) > 0:
                s += "Bones\r\n"
                s += bonestr(strs, bones, -1, 0)
                s += "\r\n"

            # actions (animations)
            if len(actions) > 0:
                for a in actions:
                    if len(a[2]) < 1:
                        continue
                    s += "Action " + str(a[1]) + " " + strs[a[0]] + "\r\n"
                    for f in a[2]:
                        s += "frame " + str(f[0]) + "\r\n"
                        for t in f[1]:
                            s += str(t[0]) + " " + str(t[1]) + " " + str(t[2]) + "\r\n"
                    s += "\r\n"

            # inlined assets
            if len(inlined) > 0:
                s += "Assets\r\n"
                for i in inlined:
                    s += strs[i[0]] + ".png\r\n"
                s += "\r\n"

            # write out file
            filepath = filepath[:len(filepath)-4] + ".a3d"
            if use_strmcompress:
                import gzip
                # could have use gzip.open, but we need the compressed size too
                s = gzip.compress(bytes(s, 'utf-8'), 9)
                filepath += ".gz"
                f = open(filepath, 'wb')
            else:
                f = open(filepath, 'w')
            f.write(s)
            f.close()
            s = len(s)
        else:
            # save Model 3D binary variant
            stridx = [0] * (len(strs))
            st = bytes(safestr(use_name, 2), 'utf-8') + pack("<b", 0)
            st = st + bytes(safestr(use_license, 2), 'utf-8') + pack("<b", 0)
            st = st + bytes(safestr(use_author, 2), 'utf-8') + pack("<b", 0)
            st = st + bytes(safestr(use_comment, 1), 'utf-8') + pack("<b", 0)
            o = len(st)
            for i, s in enumerate(strs):
                s = bytes(s, 'utf-8') + pack("<b", 0)
                st = st + s
                stridx[i] = o
                o = o + len(s)

            # construct model header chunk
            ci_s = idxsize(len(cmap))
            ti_s = idxsize(len(tmaps))
            vi_s = idxsize(len(verts))
            si_s = idxsize(o)
            bi_s = idxsize(len(bones))
            sk_s = idxsize(len(skins))
            hi_s = idxsize(len(shapes))
            fi_s = idxsize(len(faces))
            if nb_m < 2:
                nb_s = 0
            elif nb_m == 2:
                nb_s = 1
            elif nb_m <= 4:
                nb_s = 2
            else:
                nb_s = 3
            fc_s = idxsize(fi_m)
            flags = (use_quality << 0) | (vi_s << 2) | (si_s << 4) | (ci_s << 6) | (ti_s << 8) | (bi_s << 10) | (nb_s << 12)
            flags |= (sk_s << 14) | (fc_s << 16) | (hi_s << 18) | (fi_s << 20)
            buf = pack("<f", use_scale) + pack("<I", flags) + st
            buf = b'HEAD' + pack("<I",len(buf) + 8) + buf

            # color map
            if len(cmap) > 0 and ci_s < 4:
                buf = buf + b'CMAP' + pack("<I", len(cmap) * 4 + 8)
                for col in cmap:
                    for i in range(0, 4):
                        buf = buf + pack("<B", int(col[i] * 255))

            # texture map
            if len(tmaps) > 0:
                buf = buf + b'TMAP' + pack("<I", len(tmaps) * 2 * (1 << use_quality) + 8)
                r = True
                for t in tmaps:
                    # failsafes
                    if t[0] < 0.0 or t[0] > 1.0 or t[1] < 0.0 or t[1] > 1.0:
                        if r:
                            r = False
                            report({"ERROR"}, "Texture UV's are out of 0..1 range")
                        #print("Eeeeeek texture UV's are out of 0..1 range? Should never happen!", t)
                        if t[0] > 1.0:
                            t[0] = 1.0
                        if t[0] < 0.0:
                            t[0] = 0.0
                        if t[1] > 1.0:
                            t[1] = 1.0
                        if t[1] < 0.0:
                            t[1] = 0.0
                    if use_quality == 0:
                        buf = buf + pack("<BB", int(t[0] * 255), int(t[1] * 255))
                    elif use_quality == 1:
                        buf = buf + pack("<HH", int(t[0] * 65535), int(t[1] * 65535))
                    elif use_quality == 3:
                        buf = buf + pack("<dd", t[0], t[1])
                    else:
                        buf = buf + pack("<ff", t[0], t[1])

            # vertex list
            if len(verts) > 0:
                o = b''
                for v in verts:
                    for i in range(0, 4):
                        if use_quality == 0:
                            o = o + pack("<b", int(v[i] * 127))
                        elif use_quality == 1:
                            o = o + pack("<h", int(v[i] * 32767))
                        elif use_quality == 3:
                            o = o + pack("<d", v[i])
                        else:
                            o = o + pack("<f", v[i])
                    if ci_s < 4:
                        o = o + addidx(ci_s, v[4])
                    else:
                        o = o + pack("<I", cmap[v[4]])
                    o = o + addidx(sk_s, v[5])
                buf = buf + b'VRTS' + pack("<I", len(o) + 8) + o

            # skeleton
            if len(bones) > 0 or len(skins) > 0:
                o = addidx(bi_s, len(bones)) + addidx(sk_s, len(skins))
                for b in bones:
                    o = o + addidx(bi_s, b[0]) + addidx(si_s, stridx[b[1]]) + addidx(vi_s, b[2]) + addidx(vi_s, b[3])
                for s in skins:
                    if nb_s > 0:
                        for i in range(0, 1 << nb_s):
                            if i >= len(s):
                                o = o + pack("<B", 0)
                            else:
                                o = o + pack("<B", s[i][1])
                    for i in range(0, min(len(s), 1 << nb_s)):
                        if s[i][1] != 0:
                            o = o + addidx(bi_s, s[i][0])
                buf = buf + b'BONE' + pack("<I", len(o) + 8) + o

            # materials
            if len(materials) > 0:
                for m in materials:
                    o = addidx(si_s, stridx[m[0]])
                    for pi,p in m[1].items():
                        o = o + pack("<B", p[0])
                        t = mat_property_map[p[0]]
                        if t[0] == "color" or t[0] == "gscale":
                            if ci_s < 4:
                                o = o + addidx(ci_s, p[1])
                            else:
                                o = o + pack("<I", cmap[p[1]])
                        elif t[0] == "byte" or t[0] == "//byte":
                            o = o + pack("<B", p[1])
                        elif p[0] >= 128:
                            o = o + addidx(si_s, stridx[p[1]])
                        else:
                            o = o + pack("<f", p[1])
                    buf = buf + b'MTRL' + pack("<I", len(o) + 8) + o

            # triangle mesh
            if len(faces) > 0:
                l = -1
                o = b''
                for f in faces:
                    if l != f[0]:
                        l = f[0]
                        o = o + pack("<b", 0) + addidx(si_s, stridx[l])
                    o = o + pack("<b", (len(f[1]) << 4) | (use_uvs) | (use_normals << 1))
                    for i,v in enumerate(f[1]):
                        o = o + addidx(vi_s, v)
                        if use_uvs:
                            o = o + addidx(ti_s, f[2][i])
                        if use_normals:
                            o = o + addidx(vi_s, f[3][i])
                buf = buf + b'MESH' + pack("<I", len(o) + 8) + o

            # shapes
            if len(shapes) > 0:
                l = -1
                o = b''
                for f in shapes:
                    o = o + b''
                buf = buf + b'SHPE' + pack("<I", len(o) + 8) + o

            # labels
            if len(labels) > 0:
                l = -1
                o = b''
                for f in labels:
                    o = o + b''
                buf = buf + b'LBLS' + pack("<I", len(o) + 8) + o

            # actions (animations)
            if len(actions) > 0:
                for a in actions:
                    if len(a[2]) < 1:
                        continue
                    o = addidx(si_s, stridx[a[0]]) + pack("<H", len(a[2])) + pack("<I", a[1])
                    for f in a[2]:
                        o = o + pack("<I", f[0]) + addidx(fc_s, len(f[1]))
                        for t in f[1]:
                            o = o + addidx(bi_s, t[0]) + addidx(vi_s, t[1]) + addidx(vi_s, t[2])
                    buf = buf + b'ACTN' + pack("<I", len(o) + 8) + o

            # inlined assets
            if len(inlined) > 0:
                for i in inlined:
                    o = addidx(si_s, stridx[i[0]]) + i[1]
                    buf = buf + b'ASET' + pack("<I", len(o) + 8) + o

            # extra chunks
            if len(extras) > 0:
                for e in extras:
                    buf = buf + e[0][0:3] + pack("<I", len(e[1]) + 8) + e[1]

            # End chunk
            buf = buf + b'OMD3';
            if use_strmcompress:
                import zlib
                buf = zlib.compress(buf, 9)

            # add file header and write out file
            f = open(filepath, 'wb')
            s = len(buf) + 8
            f.write(b'3DMO' + pack("<L", s) + buf)
            f.close()

        progress.leave_substeps("Finished!")

        report({"INFO"}, "Model 3D " + filepath + " (" + str(s) + " bytes) exported.")
    return {'FINISHED'}


# -----------------------------------------------------------------------------
# Blender integration
import bpy
from bpy.props import (
        BoolProperty,
        FloatProperty,
        StringProperty,
        IntProperty,
        EnumProperty,
        )
from bpy_extras.io_utils import (
        ExportHelper,
        ImportHelper,
        axis_conversion,
        )

class ImportM3D(bpy.types.Operator, ImportHelper):
    """Load a Model 3D File (.m3d)"""

    bl_idname = "import_scene.m3d"
    bl_label = 'Import M3D'
    bl_options = {'PRESET'}

    filename_ext = ".m3d"
    filter_glob: StringProperty(
            default="*.m3d;*.a3d;*.a3d.gz",
            options={'HIDDEN'},
            )

    def execute(self, context):
        return read_m3d(context, self.filepath, self.report)


class ExportM3D(bpy.types.Operator, ExportHelper):
    """Save a Model 3D File (.m3d)"""

    bl_idname = "export_scene.m3d"
    bl_label = 'Export M3D'
    bl_options = {'PRESET'}

    filename_ext = ".m3d"
    filter_glob: StringProperty(
            default="*.m3d",
            options={'HIDDEN'},
            )

    # model properties
    use_name: StringProperty(
            name="Model Name",
            description="Name of the exported model",
            default="",
            )
    use_license: StringProperty(
            name="License",
            description="Licensing, copyright notice",
            default="MIT",
            )
    use_author: StringProperty(
            name="Author",
            description="Your name and contact (email, git repo url etc.)",
            default="",
            )
    use_comment: StringProperty(
            name="Comment",
            description="Any description or comment on the model",
            default="",
            )
    use_scale: FloatProperty(
            name="Scale (meter)",
            description="Specify model space 1.0 in SI meters (use 0.0 to calculate)",
            min=0.0, max=1000.0,
            default=1.0,
            )
    # import range
    use_selection: BoolProperty(
            name="Selection Only",
            description="Export selected objects only",
            default=False,
            )
    use_mesh_modifiers: BoolProperty(
            name="Apply Modifiers",
            description="Apply modifiers",
            default=True,
            )
    # export properties
    use_normals: BoolProperty(
            name="Include Normals",
            description="Export one normal per vertex and per face, to represent flat faces and sharp edges",
            default=True,
            )
    use_uvs: BoolProperty(
            name="Include UVs",
            description="Write out the active UV coordinates",
            default=True,
            )
    use_colors: BoolProperty(
            name="Include Vertex Colors",
            description="Write out individual vertex colors (independent to material colors)",
            default=True,
            )
    use_shapes: BoolProperty(
            name="Write Shapes",
            description="Write out as parameterized shapes",
            default=False,
            )
    use_materials: BoolProperty(
            name="Write Materials",
            description="Write out the materials",
            default=True,
            )
    use_skeleton: BoolProperty(
            name="Write Armature",
            description="Write out armature (bones hiearachy and skin)",
            default=True,
            )
    use_animation: BoolProperty(
            name="Write Animation",
            description="Write out actions (implies armature)",
            default=True,
            )
    use_markers: BoolProperty(
            name="Use Markers",
            description="Use timeline markers for animations instead of actions",
            default=False,
            )
    use_fps: IntProperty(
            name="FPS",
            description="Specify frame per second. Blender only nows about frames",
            min=1, max=120,
            default=25,
            )
    use_quality: EnumProperty(
            name="Precision",
            items=(('-1','auto', 'choose depending on the number of polygons'),
                   ('0', '8 bits (int8)', '1/256 coordinate unit (for low poly models)'),
                   ('1','16 bits (int16)', '1/65536 coordinate unit (more than enough in most cases)'),
                   ('2','32 bits (float)', 'float precision coordinates (used by most other binary formats)'),
                   ('3','64 bits (double)', 'double precision coordinates (rarely needed)'),
                  ),
            description="Coordinate grid system's size and precision",
            default='-1',
            )
    use_inline: BoolProperty(
            name="Embed Assets",
            description="Inline assets (like textures) into output, create a single file that contains everything",
            default=False,
            )
    use_gridcompress: BoolProperty(
            name="Use Gridcompression",
            description="Use lossy compression, achieve much smaller files by sacrificing a little bit of model quality",
            default=True,
            )
    use_strmcompress: BoolProperty(
            name="Use Streamcompression",
            description="Use lossless deflate on binary data. Unless you're writing your own M3D parser, keep it checked",
            default=True,
            )
    use_ascii: BoolProperty(
            name="Use ASCII variant",
            description="Use plain text variant of Model 3D for output",
            default=False,
            )

    def execute(self, context):
        # Exit edit mode before exporting, so current object states are exported properly.
        if bpy.ops.object.mode_set.poll():
            bpy.ops.object.mode_set(mode='OBJECT')

        keywords = self.as_keywords(ignore=("filepath", "filter_glob"))
        return write_m3d(context, self.filepath, self.report, **keywords)


def menu_func_export(self, context):
    self.layout.operator(ExportM3D.bl_idname, text="Model 3D (.m3d)")


def menu_func_import(self, context):
    self.layout.operator(ImportM3D.bl_idname, text="Model 3D (.m3d/.a3d)")


def register():
    bpy.utils.register_class(ExportM3D)
    bpy.utils.register_class(ImportM3D)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)
#    bpy.types.TOPBAR_MT_file_import.append(menu_func_import)


def unregister():
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)
#    bpy.types.TOPBAR_MT_file_import.remove(menu_func_import)
    bpy.utils.unregister_class(ExportM3D)
    bpy.utils.unregister_class(ImportM3D)


if __name__ == "__main__":
    register()
