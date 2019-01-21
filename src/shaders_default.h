#pragma once

//icon shader
static const char* g_shader_icon_vertex =
"#version 330\n"
"layout(location = 0) in vec3 a_vertex; \n"
"layout(location = 1) in vec2 a_uv; \n"
"out vec2 v_uv;\n"
"uniform mat4 u_mvp;\n"
"void main() {\n"
"    gl_Position = u_mvp * vec4(a_vertex, 1); \n"
"    v_uv = a_uv; \n"
"}\n";

static const char* g_shader_icon_fragment =
"#version 330\n"
"in vec2 v_uv;\n"
"layout(location = 0) out vec4 fragColor;\n"
"uniform sampler2D u_icon;\n"
"void main() {\n"
"    fragColor = texture(u_icon, v_uv);\n"
"}\n";


//font shader
static const char* g_shader_font_vertex =
"#version 330\n"
"layout(location = 0) in vec3 a_vertex;\n"
"layout(location = 1) in vec2 a_uv;\n"
"uniform mat4 u_mvp;\n"
"out vec2 v_uv;\n"
"void main() {\n"
"	gl_Position = u_mvp * vec4(a_vertex, 1);\n"
"	v_uv = a_uv;\n"
"}\n";

static const char* g_shader_font_fragment =
"#version 330\n"
"in vec2 v_uv;\n"
"out vec4 fragColor;\n"
"uniform sampler2D u_icon;\n"
"uniform vec3 u_color;\n"
"void main() {\n"
"	float final_color = texture(u_icon, v_uv).r;\n"
"	fragColor = vec4(u_color, final_color);\n"
"}\n";

