#include "common.h"

#include "core.h"
#include "raylib.h"

ShaderResources LoadShaders(AppConfiguration appProps)
{
	// 	shaders[FX_DEFAULT] = LoadShader(0, pathJoin(appProps.res_path, TextFormat("resources\\shaders\\glsl%i\\default.fs", appProps.glsl_version)));
	// 	shaders[FX_BLOOM] = LoadShader(0, pathJoin(appProps.res_path, TextFormat("resources\\shaders\\glsl%i\\bloom.fs", appProps.glsl_version)));

	char *shadef_path = TextFormat("resources\\shaders\\glsl%i\\default.fs", appProps.glsl_version);
	const char shadef_str[999];
	snprintf(shadef_str, sizeof(shadef_str), "%s\\%s", appProps.res_path, shadef_path);

	char *sha_path = TextFormat("resources\\shaders\\glsl%i\\bloom.fs", appProps.glsl_version);
	const char sha_str[999];
	snprintf(sha_str, sizeof(sha_str), "%s\\%s", appProps.res_path, sha_path);

	// load
	ShaderResources res;
	res.shaders[FX_DEFAULT] = LoadShader(0, shadef_str);
	res.shaders[FX_BLOOM] = LoadShader(0, sha_str);
	return res;
}

void UnloadShaders(ShaderResources res)
{
	for (int i = 0; i < MAX_POSTPRO_SHADERS; i++)
		UnloadShader(res.shaders[i]);
}