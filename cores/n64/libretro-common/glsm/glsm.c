/* Copyright (C) 2010-2019 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this libretro SDK code part (glsm).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glsym/glsym.h>
#include <glsm/glsm.h>
#include <GLideN64_libretro.h>

#if defined(HAVE_OPENGLES)
#if !defined(IOS)
#include <EGL/egl.h>
#endif // !defined(IOS)
typedef void (GL_APIENTRYP PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void (GL_APIENTRYP PFNGLBUFFERSTORAGEEXTPROC) (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
typedef void (GL_APIENTRYP PFNGLMEMORYBARRIERPROC) (GLbitfield barriers);
typedef void (GL_APIENTRYP PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (GL_APIENTRYP PFNGLTEXSTORAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GL_APIENTRYP PFNGLCOPYIMAGESUBDATAPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC m_glDrawRangeElementsBaseVertex;
PFNGLBUFFERSTORAGEEXTPROC m_glBufferStorage;
PFNGLMEMORYBARRIERPROC m_glMemoryBarrier;
PFNGLBINDIMAGETEXTUREPROC m_glBindImageTexture;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC m_glTexStorage2DMultisample;
PFNGLCOPYIMAGESUBDATAPROC m_glCopyImageSubData;
#endif // defined(HAVE_OPENGLES)

#ifndef GL_DEPTH_CLAMP
#define GL_DEPTH_CLAMP                    0x864F
#define GL_RASTERIZER_DISCARD             0x8C89
#define GL_SAMPLE_MASK                    0x8E51
#endif

#define MAX_FRAMEBUFFERS 128000
#define MAX_UNIFORMS 1024

#if 0
extern retro_log_printf_t log_cb;
#define GLSM_DEBUG
#endif

struct gl_cached_state
{
   struct
   {
      GLuint ids[32];
      GLenum target[32];
   } bind_textures;


   struct
   {
      bool used[MAX_ATTRIB];
      GLint size[MAX_ATTRIB];
      GLenum type[MAX_ATTRIB];
      GLboolean normalized[MAX_ATTRIB];
      GLsizei stride[MAX_ATTRIB];
      const GLvoid *pointer[MAX_ATTRIB];
      GLuint buffer[MAX_ATTRIB];
   } attrib_pointer;

#ifndef HAVE_OPENGLES
   GLenum colorlogicop;
#endif

   struct
   {
      bool enabled[MAX_ATTRIB];
   } vertex_attrib_pointer;
   
   struct
   {
      GLuint array;
    } bindvertex;

   struct
   {
      GLenum pname;
      GLint param;
   } pixelstore_i;
   
   struct
   {
     GLint pack;
     GLint unpack;
   } pixelstore;

   struct
   {
      GLuint r;
      GLuint g;
      GLuint b;
      GLuint a;
   } clear_color;

   struct
   {
      bool used;
      GLint x;
      GLint y;
      GLsizei w;
      GLsizei h;
   } scissor;

   struct
   {
      GLint x;
      GLint y;
      GLsizei w;
      GLsizei h;
   } viewport;

   struct
   {
      bool used;
      GLenum sfactor;
      GLenum dfactor;
   } blendfunc;

   struct
   {
      bool used;
      GLenum srcRGB;
      GLenum dstRGB;
      GLenum srcAlpha;
      GLenum dstAlpha;
   } blendfunc_separate;

   struct
   {
      bool used;
      GLboolean red;
      GLboolean green;
      GLboolean blue;
      GLboolean alpha;
   } colormask;

   struct
   {
      bool used;
      GLdouble depth;
   } cleardepth;

   struct
   {
      bool used;
      GLenum func;
   } depthfunc;

   struct
   {
      bool used;
      GLclampd zNear;
      GLclampd zFar;
   } depthrange;

   struct
   {
      bool used;
      GLfloat factor;
      GLfloat units;
   } polygonoffset;

   struct
   {
      bool used;
      GLenum func;
      GLint ref;
      GLuint mask;
   } stencilfunc;

   struct
   {
      bool used;
      GLenum sfail;
      GLenum dpfail;
      GLenum dppass;
   } stencilop;

   struct
   {
      bool used;
      GLenum mode;
   } frontface;

   struct
   {
      bool used;
      GLenum mode;
   } cullface;

   struct
   {
      bool used;
      GLuint mask;
   } stencilmask;

   struct
   {
      bool used;
      GLboolean mask;
   } depthmask;

   struct
   {
      GLenum mode;
   } readbuffer;

   struct
   {
      GLuint location;
      GLuint desired_location;
   } framebuf[2];

   GLuint vao;
   GLuint array_buffer;
   GLuint index_buffer;
   GLuint program;
   GLenum active_texture;
   int cap_state[SGL_CAP_MAX];
   int cap_translate[SGL_CAP_MAX];
};

struct gl_program_uniforms
{
   GLfloat uniform1f;
   GLfloat uniform2f[2];
   GLfloat uniform3f[3];
   GLfloat uniform4f[4];
   GLint uniform1i;
   GLint uniform2i[2];
   GLint uniform3i[3];
   GLint uniform4i[4];
};

struct gl_framebuffers
{
   int draw_buf_set;
   GLuint color_attachment;
   GLuint depth_attachment;
   GLenum target;
};

static GLuint default_framebuffer;
static GLint glsm_max_textures;
struct retro_hw_render_callback hw_render;
static struct gl_cached_state gl_state;

static bool copy_image_support = 0;
static struct gl_program_uniforms program_uniforms[MAX_UNIFORMS][MAX_UNIFORMS];
static struct gl_framebuffers* framebuffers[MAX_FRAMEBUFFERS];

static GLenum active_texture;
static int window_first = 0;
static int resetting_context = 0;
extern void retroChangeWindow();

void bindFBO(GLenum target)
{
#ifdef HAVE_OPENGLES2
   if (target == GL_FRAMEBUFFER && (gl_state.framebuf[0].desired_location != gl_state.framebuf[0].location || gl_state.framebuf[1].desired_location != gl_state.framebuf[1].location))
   {
      glBindFramebuffer(GL_FRAMEBUFFER, gl_state.framebuf[0].desired_location);
      gl_state.framebuf[0].location = gl_state.framebuf[0].desired_location;
      gl_state.framebuf[1].location = gl_state.framebuf[1].desired_location;
   }
#else
   if ((target == GL_FRAMEBUFFER || target == GL_DRAW_FRAMEBUFFER) && gl_state.framebuf[0].desired_location != gl_state.framebuf[0].location)
   {
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gl_state.framebuf[0].desired_location);
      gl_state.framebuf[0].location = gl_state.framebuf[0].desired_location;
   }
   else if (target == GL_READ_FRAMEBUFFER && gl_state.framebuf[1].desired_location != gl_state.framebuf[1].location)
   {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, gl_state.framebuf[1].desired_location);
      gl_state.framebuf[1].location = gl_state.framebuf[1].desired_location;
   }
#endif
}

/* GL wrapper-side */

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
GLenum rglGetError(void)
{
   return glGetError();
}

/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : N/A
 */

void rglProvokingVertex(	GLenum provokeMode)
{
#if defined(HAVE_OPENGL)
   glProvokingVertex(provokeMode);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : 3.0
 */
void rglGetInteger64v(	GLenum pname, int64_t *data)
{
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGetInteger64v(pname, (GLint64*)data);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : 3.0
 */
void rglSamplerParameteri(	GLuint sampler,
 	GLenum pname,
 	GLint param)
{
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glSamplerParameteri(sampler, pname, param);
#endif
}

void rglGenSamplers(	GLsizei n,
 	GLuint *samplers)
{
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGenSamplers(n, samplers);
#endif
}

void rglBindSampler(	GLuint unit,
 	GLuint sampler)
{
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glBindSampler(unit, sampler);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglClear(GLbitfield mask)
{
   bindFBO(GL_FRAMEBUFFER);
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glClear.\n");
#endif
   glClear(mask);
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglValidateProgram(GLuint program)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glValidateProgram.\n");
#endif
   glValidateProgram(program);
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 * OpenGLES  : N/A
 */
void rglPolygonMode(GLenum face, GLenum mode)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glPolygonMode.\n");
#endif
#ifndef HAVE_OPENGLES
   glPolygonMode(face, mode);
#endif
}

void rglTexSubImage2D(
      GLenum target,
  	GLint level,
  	GLint xoffset,
  	GLint yoffset,
  	GLsizei width,
  	GLsizei height,
  	GLenum format,
  	GLenum type,
  	const GLvoid * pixels)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glTexSubImage2D.\n");
#endif
   glTexSubImage2D(target, level, xoffset, yoffset,
         width, height, format, type, pixels);
}

void rglGetBufferSubData(	GLenum target,
 	GLintptr offset,
 	GLsizeiptr size,
 	GLvoid * data)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetBufferSubData.\n");
#endif
#if defined(HAVE_OPENGL)
   glGetBufferSubData(target, offset, size, data);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglLineWidth(GLfloat width)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glLineWidth.\n");
#endif
   glLineWidth(width);
}

void rglReadPixels(	GLint x,
 	GLint y,
 	GLsizei width,
 	GLsizei height,
 	GLenum format,
 	GLenum type,
 	GLvoid * data)
{
#ifdef HAVE_OPENGLES2
   bindFBO(GL_FRAMEBUFFER);
#else
   bindFBO(GL_READ_FRAMEBUFFER);
#endif
   glReadPixels(x, y, width, height, format, type, data);
}

/*
 *
 * Core in:
 * OpenGLES  : 3.0
 */
void rglReadBuffer(GLenum mode)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glReadBuffer.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glReadBuffer(mode);
   gl_state.readbuffer.mode = mode;
#endif
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglClearDepth(GLdouble depth)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glClearDepth.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
#ifdef HAVE_OPENGLES
   glClearDepthf(depth);
#else
   glClearDepth(depth);
#endif
   gl_state.cleardepth.used  = true;
   gl_state.cleardepth.depth = depth;
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglPixelStorei(GLenum pname, GLint param)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glPixelStorei.\n");
#endif
   if (pname == GL_UNPACK_ALIGNMENT) {
      if (param != gl_state.pixelstore.unpack) {
         glPixelStorei(pname, param);
         gl_state.pixelstore.unpack = param;
      }
   } else if (pname == GL_PACK_ALIGNMENT) {
      if (param != gl_state.pixelstore.pack) {
         glPixelStorei(pname, param);
         gl_state.pixelstore.pack = param;
      }
   } else
      glPixelStorei(pname, param);
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglDepthRange(GLclampd zNear, GLclampd zFar)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDepthRange.\n");
#endif
#ifdef HAVE_OPENGLES
   glDepthRangef(zNear, zFar);
#else
   glDepthRange(zNear, zFar);
#endif
   gl_state.depthrange.used  = true;
   gl_state.depthrange.zNear = zNear;
   gl_state.depthrange.zFar  = zFar;
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglFrontFace(GLenum mode)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glFrontFace.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   gl_state.frontface.used = true;
   if (gl_state.frontface.mode != mode) {
      glFrontFace(mode);
      gl_state.frontface.mode = mode;
   }
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglDepthFunc(GLenum func)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDepthFunc.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   gl_state.depthfunc.used = true;
   if (gl_state.depthfunc.func != func) {
      glDepthFunc(func);
      gl_state.depthfunc.func = func;
   }
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglColorMask(GLboolean red, GLboolean green,
      GLboolean blue, GLboolean alpha)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glColorMask.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   gl_state.colormask.used  = true;
   if (gl_state.colormask.red != red || gl_state.colormask.green != green || gl_state.colormask.blue != blue || gl_state.colormask.alpha != alpha) {
       glColorMask(red, green, blue, alpha);
       gl_state.colormask.red   = red;
       gl_state.colormask.green = green;
       gl_state.colormask.blue  = blue;
       gl_state.colormask.alpha = alpha;
   }
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglCullFace(GLenum mode)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glCullFace.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   if (gl_state.cullface.mode != mode) {
      glCullFace(mode);
      gl_state.cullface.mode = mode;
   }
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glStencilOp.\n");
#endif
   gl_state.stencilop.used   = true;
   if (gl_state.stencilop.sfail != sfail || gl_state.stencilop.dpfail != dpfail || gl_state.stencilop.dppass != dppass) {
      glStencilOp(sfail, dpfail, dppass);
      gl_state.stencilop.sfail  = sfail;
      gl_state.stencilop.dpfail = dpfail;
      gl_state.stencilop.dppass = dppass;
   }
}

/*
 *
 * Core in:
 * OpenGLES  : 2.0
 */
void rglStencilFunc(GLenum func, GLint ref, GLuint mask)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glStencilFunc.\n");
#endif
   gl_state.stencilfunc.used = true;
   if (gl_state.stencilfunc.func != func || gl_state.stencilfunc.ref != ref || gl_state.stencilfunc.mask != mask) {
      glStencilFunc(func, ref, mask);
      gl_state.stencilfunc.func = func;
      gl_state.stencilfunc.ref  = ref;
      gl_state.stencilfunc.mask = mask;
   }
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
GLboolean rglIsEnabled(GLenum cap)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glIsEnabled.\n");
#endif
   return gl_state.cap_state[cap] ? GL_TRUE : GL_FALSE;
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglClearColor(GLclampf red, GLclampf green,
      GLclampf blue, GLclampf alpha)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glClearColor.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   if (gl_state.clear_color.r != red || gl_state.clear_color.g != green || gl_state.clear_color.b != blue || gl_state.clear_color.a != alpha) {
      glClearColor(red, green, blue, alpha);
      gl_state.clear_color.r = red;
      gl_state.clear_color.g = green;
      gl_state.clear_color.b = blue;
      gl_state.clear_color.a = alpha;
   }
}

/*
 *
 * Core in:
 * OpenGLES    : 2.0 (maybe earlier?)
 */
void rglScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glScissor.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   gl_state.scissor.used = true;
   if (gl_state.scissor.x != x || gl_state.scissor.y != y || gl_state.scissor.w != width || gl_state.scissor.h != height) {
      glScissor(x, y, width, height);
      gl_state.scissor.x = x;
      gl_state.scissor.y = y;
      gl_state.scissor.w = width;
      gl_state.scissor.h = height;
   }
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glViewport.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);

   if (gl_state.viewport.x != x || gl_state.viewport.y != y || gl_state.viewport.w != width || gl_state.viewport.h != height) {
      glViewport(x, y, width, height);
      gl_state.viewport.x = x;
      gl_state.viewport.y = y;
      gl_state.viewport.w = width;
      gl_state.viewport.h = height;
   }
}

void rglBlendFunc(GLenum sfactor, GLenum dfactor)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBlendFunc.\n");
#endif
   gl_state.blendfunc.used = true;
   if (gl_state.blendfunc.sfactor != sfactor || gl_state.blendfunc.dfactor != dfactor) {
      glBlendFunc(sfactor, dfactor);
      gl_state.blendfunc.sfactor = sfactor;
      gl_state.blendfunc.dfactor = dfactor;
   }
}

/*
 * Category: Blending
 *
 * Core in:
 * OpenGL    : 1.4
 */
void rglBlendFuncSeparate(GLenum sfactor, GLenum dfactor)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBlendFuncSeparate.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   gl_state.blendfunc_separate.used     = true;
   gl_state.blendfunc_separate.srcRGB   = sfactor;
   gl_state.blendfunc_separate.dstRGB   = dfactor;
   gl_state.blendfunc_separate.srcAlpha = sfactor;
   gl_state.blendfunc_separate.dstAlpha = dfactor;
   glBlendFunc(sfactor, dfactor);
}

/*
 * Category: Textures
 *
 * Core in:
 * OpenGL    : 1.3
 */
void rglActiveTexture(GLenum texture)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glActiveTexture. %x\n", texture);
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   if (active_texture != texture - GL_TEXTURE0) {
      bindFBO(GL_FRAMEBUFFER);
      glActiveTexture(texture);
      active_texture = texture - GL_TEXTURE0;
   }
}

/*
 *
 * Core in:
 * OpenGL    : 1.1
 */
void rglBindTexture(GLenum target, GLuint texture)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBindTexture. %x, %x\n", target, texture);
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   if (gl_state.bind_textures.ids[active_texture] != texture || gl_state.bind_textures.target[active_texture] != target) {
      bindFBO(GL_FRAMEBUFFER);
      glBindTexture(target, texture);
      gl_state.bind_textures.ids[active_texture] = texture;
      gl_state.bind_textures.target[active_texture] = target;
   }
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglDisable(GLenum cap)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDisable. %x\n", cap);
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   if (gl_state.cap_state[cap] != 0) {
      glDisable(gl_state.cap_translate[cap]);
      gl_state.cap_state[cap] = 0;
   }
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglEnable(GLenum cap)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glEnable. %x\n", cap);
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   if (gl_state.cap_state[cap] != 1) {
      glEnable(gl_state.cap_translate[cap]);
      gl_state.cap_state[cap] = 1;
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUseProgram(GLuint program)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUseProgram.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);

   if(gl_state.program != program)
   {
      gl_state.program = program;
      glUseProgram(program);
   }
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglDepthMask(GLboolean flag)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDepthMask.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   glDepthMask(flag);
   gl_state.depthmask.used = true;
   gl_state.depthmask.mask = flag;
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglStencilMask(GLenum mask)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glStencilMask.\n");
#endif
   glStencilMask(mask);
   gl_state.stencilmask.used = true;
   gl_state.stencilmask.mask = mask;
}

/*
 *
 * Core in:
 * OpenGL    : 1.5
 */
void rglBufferData(GLenum target, GLsizeiptr size,
      const GLvoid *data, GLenum usage)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBufferData.\n");
#endif
   glBufferData(target, size, data, usage);
}

/*
 *
 * Core in:
 * OpenGL    : 1.5
 */
void rglBufferSubData(GLenum target, GLintptr offset,
      GLsizeiptr size, const GLvoid *data)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBufferSubData.\n");
#endif
   glBufferSubData(target, offset, size, data);
}

/*
 *
 * Core in:
 * OpenGL    : 1.5
 */
void rglBindBuffer(GLenum target, GLuint buffer)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBindBuffer.\n");
#endif
   if (target == GL_ARRAY_BUFFER) {
      if (gl_state.array_buffer != buffer) {
         gl_state.array_buffer = buffer;
         glBindBuffer(target, buffer);
      }
   }
   else if (target == GL_ELEMENT_ARRAY_BUFFER) {
      if (gl_state.index_buffer != buffer) {
         gl_state.index_buffer = buffer;
         glBindBuffer(target, buffer);
      }
   }
   else
      glBindBuffer(target, buffer);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglLinkProgram(GLuint program)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glLinkProgram.\n");
#endif
   glLinkProgram(program);
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 2.0
 */
void rglFramebufferTexture2D(GLenum target, GLenum attachment,
      GLenum textarget, GLuint texture, GLint level)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glFramebufferTexture2D.\n");
#endif
   int type;
   if (target == GL_FRAMEBUFFER)
      type = 0;
#ifndef HAVE_OPENGLES2
   else if (target == GL_DRAW_FRAMEBUFFER)
      type = 0;
   else if (target == GL_READ_FRAMEBUFFER)
      type = 1;
#endif
   if (gl_state.framebuf[type].desired_location < MAX_FRAMEBUFFERS) {
      framebuffers[gl_state.framebuf[type].desired_location]->target = textarget;
      if (attachment == GL_COLOR_ATTACHMENT0) {
         if (framebuffers[gl_state.framebuf[type].desired_location]->color_attachment != texture) {
            bindFBO(target);
            glFramebufferTexture2D(target, attachment, textarget, texture, level);
            framebuffers[gl_state.framebuf[type].location]->color_attachment = texture;
         }
      }
      else if (attachment == GL_DEPTH_ATTACHMENT) {
         if (framebuffers[gl_state.framebuf[type].desired_location]->depth_attachment != texture) {
            bindFBO(target);
            glFramebufferTexture2D(target, attachment, textarget, texture, level);
            framebuffers[gl_state.framebuf[type].location]->depth_attachment = texture;
         }
      }
   } else {
      bindFBO(target);
      glFramebufferTexture2D(target, attachment, textarget, texture, level);
   }
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 3.2
 */
void rglFramebufferTexture(GLenum target, GLenum attachment,
  	GLuint texture, GLint level)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glFramebufferTexture.\n");
#endif
   bindFBO(target);
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES_3_2)
   glFramebufferTexture(target, attachment, texture, level);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 1.1
 */
void rglDrawArrays(GLenum mode, GLint first, GLsizei count)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDrawArrays.\n");
#endif
   bindFBO(GL_FRAMEBUFFER);
   glDrawArrays(mode, first, count);
}

/*
 *
 * Core in:
 * OpenGL    : 1.1
 */
void rglDrawElements(GLenum mode, GLsizei count, GLenum type,
                           const GLvoid * indices)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDrawElements.\n");
#endif
   bindFBO(GL_FRAMEBUFFER);
   glDrawElements(mode, count, type, indices);
}

void rglCompressedTexImage2D(GLenum target, GLint level,
      GLenum internalformat, GLsizei width, GLsizei height,
      GLint border, GLsizei imageSize, const GLvoid *data)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glCompressedTexImage2D.\n");
#endif
   glCompressedTexImage2D(target, level, internalformat,
         width, height, border, imageSize, data);
}

void rglDeleteFramebuffers(GLsizei n, const GLuint *_framebuffers)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteFramebuffers.\n");
#endif
   int i, p;
   for (i = 0; i < n; ++i) {
      if (_framebuffers[i] < MAX_FRAMEBUFFERS) {
         free(framebuffers[_framebuffers[i]]);
         framebuffers[_framebuffers[i]] = NULL;
      }
      for (p = 0; p < 2; ++p) {
         if (_framebuffers[i] == gl_state.framebuf[p].location)
            gl_state.framebuf[p].location = 0;
      }
   }
   glDeleteFramebuffers(n, _framebuffers);
}

void rglDeleteTextures(GLsizei n, const GLuint *textures)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteTextures.\n");
#endif
   int i, p;
   for (i = 0; i < n; ++i) {
      if (textures[i] == gl_state.bind_textures.ids[active_texture]) {
         gl_state.bind_textures.ids[active_texture] = 0;
         gl_state.bind_textures.target[active_texture] = GL_TEXTURE_2D;
      }
      for (p = 0; p < MAX_FRAMEBUFFERS; ++p) {
         if (framebuffers[p] != NULL) {
            if (framebuffers[p]->color_attachment == textures[i])
               framebuffers[p]->color_attachment = 0;
            if (framebuffers[p]->depth_attachment == textures[i])
               framebuffers[p]->depth_attachment = 0;
         }
      }
   }
   glDeleteTextures(n, textures);
}

/*
 *
 * Core in:
 * OpenGLES    : 2.0
 */
void rglRenderbufferStorage(GLenum target, GLenum internalFormat,
      GLsizei width, GLsizei height)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glRenderbufferStorage.\n");
#endif
   glRenderbufferStorage(target, internalFormat, width, height);
}

/*
 *
 * Core in:
 *
 * OpenGL      : 3.0
 * OpenGLES    : 2.0
 */
void rglBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBindRenderbuffer.\n");
#endif
   glBindRenderbuffer(target, renderbuffer);
}

/*
 *
 * Core in:
 *
 * OpenGLES    : 2.0
 */
void rglDeleteRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteRenderbuffers.\n");
#endif
   glDeleteRenderbuffers(n, renderbuffers);
}

/*
 *
 * Core in:
 *
 * OpenGL      : 3.0
 * OpenGLES    : 2.0
 */
void rglGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGenRenderbuffers.\n");
#endif
   glGenRenderbuffers(n, renderbuffers);
}

/*
 *
 * Core in:
 *
 * OpenGL      : 3.0
 * OpenGLES    : 2.0
 */
void rglGenerateMipmap(GLenum target)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGenerateMipmap.\n");
#endif
   glGenerateMipmap(target);
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 3.0
 */
GLenum rglCheckFramebufferStatus(GLenum target)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glCheckFramebufferStatus.\n");
#endif
   bindFBO(target);
   return glCheckFramebufferStatus(target);
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 2.0
 */
void rglFramebufferRenderbuffer(GLenum target, GLenum attachment,
      GLenum renderbuffertarget, GLuint renderbuffer)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glFramebufferRenderbuffer.\n");
#endif
   int type;
   if (target == GL_FRAMEBUFFER)
      type = 0;
#ifndef HAVE_OPENGLES2
   else if (target == GL_DRAW_FRAMEBUFFER)
      type = 0;
   else if (target == GL_READ_FRAMEBUFFER)
      type = 1;
#endif
   if (gl_state.framebuf[type].desired_location < MAX_FRAMEBUFFERS) {
      framebuffers[gl_state.framebuf[type].desired_location]->target = renderbuffertarget;
      if (attachment == GL_COLOR_ATTACHMENT0) {
         if (framebuffers[gl_state.framebuf[type].desired_location]->color_attachment != renderbuffer) {
            bindFBO(target);
            glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
            framebuffers[gl_state.framebuf[type].location]->color_attachment = renderbuffer;
         }
      }
      else if (attachment == GL_DEPTH_ATTACHMENT) {
         if (framebuffers[gl_state.framebuf[type].desired_location]->depth_attachment != renderbuffer) {
            bindFBO(target);
            glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
            framebuffers[gl_state.framebuf[type].location]->depth_attachment = renderbuffer;
         }
      }
   } else {
      bindFBO(target);
      glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 3.0
 */
void rglBindFragDataLocation(GLuint program, GLuint colorNumber,
                                   const char * name)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBindFragDataLocation.\n");
#endif
#if !defined(HAVE_OPENGLES2) && !defined(HAVE_OPENGLES3)
   glBindFragDataLocation(program, colorNumber, name);
#endif
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglGetProgramiv(GLuint shader, GLenum pname, GLint *params)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetProgramiv.\n");
#endif
   glGetProgramiv(shader, pname, params);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 4.1
 * OpenGLES  : 3.0
 */
void rglProgramParameteri( 	GLuint program,
  	GLenum pname,
  	GLint value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glProgramParameteri.\n");
#endif
#if !defined(HAVE_OPENGLES) || defined(HAVE_OPENGLES) && (defined(HAVE_OPENGLES3) || defined(HAVE_OPENGLES_3_1))
   glProgramParameteri(program, pname, value);
#else
   printf("WARNING! Not implemented.\n");
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize,
      GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetActiveUniform.\n");
#endif
   glGetActiveUniform(program, index, bufsize, length, size, type, name);
}

void rglGenQueries(	GLsizei n,
 	GLuint * ids)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGenQueries.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGenQueries(n, ids);
#endif
}

void rglGetQueryObjectuiv(	GLuint id,
 	GLenum pname,
 	GLuint * params)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetQueryObjectuiv.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGetQueryObjectuiv(id, pname, params);
#endif
}

void rglDeleteQueries(	GLsizei n,
 	const GLuint * ids)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteQueries.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glDeleteQueries(n, ids);
#endif
}

void rglBeginQuery(	GLenum target,
 	GLuint id)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBeginQuery.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glBeginQuery(target, id);
#endif
}

void rglEndQuery(	GLenum target)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glEndQuery.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glEndQuery(target);
#endif
}

/*
 * Category: UBO
 *
 * Core in:
 *
 * OpenGL    : 2.0
 * OpenGLES  : 3.0
 */
void rglGetActiveUniformBlockiv(GLuint program,
  	GLuint uniformBlockIndex,
  	GLenum pname,
  	GLint *params)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetActiveUniformBlockiv.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGetActiveUniformBlockiv(program, uniformBlockIndex,
         pname, params);
#else
   printf("WARNING! Not implemented.\n");
#endif
}

/*
 *
 * Core in:
 *
 * OpenGLES  : 3.0
 */
void rglGetActiveUniformsiv( 	GLuint program,
  	GLsizei uniformCount,
  	const GLuint *uniformIndices,
  	GLenum pname,
  	GLint *params)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetActiveUniformsiv.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGetActiveUniformsiv(program, uniformCount,
         uniformIndices, pname, params);
#else
   printf("WARNING! Not implemented.\n");
#endif
}

/*
 *
 * Core in:
 *
 * OpenGLES  : 3.0
 */
void rglGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetUniformIndices.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGetUniformIndices(program, uniformCount,
         uniformNames, uniformIndices);
#else
   printf("WARNING! Not implemented.\n");
#endif
}

/*
 * Category: UBO
 *
 * Core in:
 *
 * OpenGLES  : 3.0
 */
void rglBindBufferBase( 	GLenum target,
  	GLuint index,
  	GLuint buffer)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBindBufferBase.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glBindBufferBase(target, index, buffer);
#else
   printf("WARNING! Not implemented.\n");
#endif
}

/*
 *
 * Category: UBO
 *
 * Core in:
 *
 * OpenGLES  : 3.0
 */
GLuint rglGetUniformBlockIndex( 	GLuint program,
  	const GLchar *uniformBlockName)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetUniformBlockIndex.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   return glGetUniformBlockIndex(program, uniformBlockName);
#else
   printf("WARNING! Not implemented.\n");
   return 0;
#endif
}

/*
 * Category: UBO
 *
 * Core in:
 *
 * OpenGLES  : 3.0
 */
void rglUniformBlockBinding( 	GLuint program,
  	GLuint uniformBlockIndex,
  	GLuint uniformBlockBinding)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniformBlockBinding.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glUniformBlockBinding(program, uniformBlockIndex,
         uniformBlockBinding);
#else
   printf("WARNING! Not implemented.\n");
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 3.0
 */
void rglUniform1ui(GLint location, GLuint v)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform1ui.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glUniform1ui(location ,v);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 3.0
 */
void rglUniform2ui(GLint location, GLuint v0, GLuint v1)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform2ui.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glUniform2ui(location, v0, v1);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 3.0
 */
void rglUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform3ui.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glUniform3ui(location, v0, v1, v2);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 3.0
 */
void rglUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform4ui.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glUniform4ui(location, v0, v1, v2, v3);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
      const GLfloat *value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniformMatrix4fv.\n");
#endif
   glUniformMatrix4fv(location, count, transpose, value);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglDetachShader(GLuint program, GLuint shader)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDetachShader.\n");
#endif
   glDetachShader(program, shader);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetShaderiv.\n");
#endif
   glGetShaderiv(shader, pname, params);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglAttachShader(GLuint program, GLuint shader)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glAttachShader.\n");
#endif
   glAttachShader(program, shader);
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
GLint rglGetAttribLocation(GLuint program, const GLchar *name)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetAttribLocation.\n");
#endif
   return glGetAttribLocation(program, name);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glShaderSource.\n");
#endif
   return glShaderSource(shader, count, string, length);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglCompileShader(GLuint shader)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glCompileShader.\n");
#endif
   glCompileShader(shader);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
GLuint rglCreateProgram(void)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glCreateProgram.\n");
#endif
   GLuint temp = glCreateProgram();
   int i;
   for (i = 0; i < MAX_UNIFORMS; ++i)
      memset(&program_uniforms[temp][i], 0, sizeof(struct gl_program_uniforms));
   return temp;
}

/*
 *
 * Core in:
 * OpenGL    : 1.1
 */
void rglGenTextures(GLsizei n, GLuint *textures)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGenTextures.\n");
#endif
   glGenTextures(n, textures);
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglGetShaderInfoLog(GLuint shader, GLsizei maxLength,
      GLsizei *length, GLchar *infoLog)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetShaderInfoLog.\n");
#endif
   glGetShaderInfoLog(shader, maxLength, length, infoLog);
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglGetProgramInfoLog(GLuint shader, GLsizei maxLength,
      GLsizei *length, GLchar *infoLog)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetProgramInfoLog.\n");
#endif
   glGetProgramInfoLog(shader, maxLength, length, infoLog);
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
GLboolean rglIsProgram(GLuint program)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glIsProgram.\n");
#endif
   return glIsProgram(program);
}

void rglTexCoord2f(GLfloat s, GLfloat t)
{
#ifdef HAVE_LEGACY_GL
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glTexCoord2f.\n");
#endif
   glTexCoord2f(s, t);
#endif
}

/*
 * Category: Generic vertex attributes
 *
 * Core in:
 * OpenGL    : 2.0
 *
 */
void rglDisableVertexAttribArray(GLuint index)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDisableVertexAttribArray.\n");
#endif
   gl_state.vertex_attrib_pointer.enabled[index] = 0;
   glDisableVertexAttribArray(index);
}

/*
 * Category: Generic vertex attributes
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglEnableVertexAttribArray(GLuint index)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glEnableVertexAttribArray.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   gl_state.vertex_attrib_pointer.enabled[index] = 1;
   glEnableVertexAttribArray(index);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglVertexAttribIPointer(
      GLuint index,
      GLint size,
      GLenum type,
      GLsizei stride,
      const GLvoid * pointer)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glVertexAttribIPointer.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glVertexAttribIPointer(index, size, type, stride, pointer);
#endif
}

void rglVertexAttribLPointer(
      GLuint index,
      GLint size,
      GLenum type,
      GLsizei stride,
      const GLvoid * pointer)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glVertexAttribLPointer.\n");
#endif
#if defined(HAVE_OPENGL)
   glVertexAttribLPointer(index, size, type, stride, pointer);
#endif
}

/*
 * Category: Generic vertex attributes
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglVertexAttribPointer(GLuint name, GLint size,
      GLenum type, GLboolean normalized, GLsizei stride,
      const GLvoid* pointer)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glVertexAttribPointer.\n");
#endif
   gl_state.attrib_pointer.used[name] = 1;
   gl_state.attrib_pointer.size[name] = size;
   gl_state.attrib_pointer.type[name] = type;
   gl_state.attrib_pointer.normalized[name] = normalized;
   gl_state.attrib_pointer.stride[name] = stride;
   gl_state.attrib_pointer.pointer[name] = pointer;
   gl_state.attrib_pointer.buffer[name] = gl_state.array_buffer;
   glVertexAttribPointer(name, size, type, normalized, stride, pointer);
}

/*
 * Category: Generic vertex attributes
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglBindAttribLocation(GLuint program, GLuint index, const GLchar *name)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBindAttribLocation.\n");
#endif
   glBindAttribLocation(program, index, name);
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglVertexAttrib4f(GLuint name, GLfloat x, GLfloat y,
      GLfloat z, GLfloat w)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glVertexAttrib4f.\n");
#endif
   glVertexAttrib4f(name, x, y, z, w);
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglVertexAttrib4fv(GLuint name, const GLfloat* v)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glVertexAttrib4fv.\n");
#endif
   glVertexAttrib4fv(name, v);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
GLuint rglCreateShader(GLenum shaderType)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glCreateShader.\n");
#endif
   return glCreateShader(shaderType);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglDeleteProgram(GLuint program)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteProgram.\n");
#endif
   if (!resetting_context)
      glDeleteProgram(program);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglDeleteShader(GLuint shader)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteShader.\n");
#endif
   if (!resetting_context)
      glDeleteShader(shader);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
GLint rglGetUniformLocation(GLuint program, const GLchar *name)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetUniformLocation.\n");
#endif
   return glGetUniformLocation(program, name);
}

/*
 * Category: VBO and PBO
 *
 * Core in:
 * OpenGL    : 1.5
 */
void rglDeleteBuffers(GLsizei n, const GLuint *buffers)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteBuffers.\n");
#endif
   glDeleteBuffers(n, buffers);
}

/*
 * Category: VBO and PBO
 *
 * Core in:
 * OpenGL    : 1.5
 */
void rglGenBuffers(GLsizei n, GLuint *buffers)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGenBuffers.\n");
#endif
   glGenBuffers(n, buffers);
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform1f(GLint location, GLfloat v0)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform1f.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform1f != v0) {
      glUniform1f(location, v0);
      program_uniforms[gl_state.program][location].uniform1f = v0;
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform1fv(GLint location,  GLsizei count,  const GLfloat *value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform1fv.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform1f != value[0]) {
      glUniform1fv(location, count, value);
      program_uniforms[gl_state.program][location].uniform1f = value[0];
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform1iv(GLint location,  GLsizei count,  const GLint *value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform1iv.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform1i != value[0]) {
      glUniform1iv(location, count, value);
      program_uniforms[gl_state.program][location].uniform1i = value[0];
   }
}

void rglClearBufferfv( 	GLenum buffer,
  	GLint drawBuffer,
  	const GLfloat * value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glClearBufferfv.\n");
#endif
   bindFBO(GL_FRAMEBUFFER);
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES_3)
   glClearBufferfv(buffer, drawBuffer, value);
#endif
}

void rglTexBuffer(GLenum target, GLenum internalFormat, GLuint buffer)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glTexBuffer.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES_3_2)
   glTexBuffer(target, internalFormat, buffer);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 3.0
 */
const GLubyte* rglGetStringi(GLenum name, GLuint index)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetString.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES_3)
   return glGetStringi(name, index);
#else
   return NULL;
#endif
}

void rglClearBufferfi( 	GLenum buffer,
  	GLint drawBuffer,
  	GLfloat depth,
  	GLint stencil)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glClearBufferfi.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES_3)
   glClearBufferfi(buffer, drawBuffer, depth, stencil);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 3.0
 */
void rglRenderbufferStorageMultisample( 	GLenum target,
  	GLsizei samples,
  	GLenum internalformat,
  	GLsizei width,
  	GLsizei height)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glRenderbufferStorageMultisample.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES_3)
   glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
#endif
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform1i(GLint location, GLint v0)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform1i.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform1i != v0) {
      glUniform1i(location, v0);
      program_uniforms[gl_state.program][location].uniform1i = v0;
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform2f.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform2f[0] != v0 || program_uniforms[gl_state.program][location].uniform2f[1] != v1) {
      glUniform2f(location, v0, v1);
      program_uniforms[gl_state.program][location].uniform2f[0] = v0;
      program_uniforms[gl_state.program][location].uniform2f[1] = v1;
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform2i(GLint location, GLint v0, GLint v1)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform2i.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform2i[0] != v0 || program_uniforms[gl_state.program][location].uniform2i[1] != v1) {
      glUniform2i(location, v0, v1);
      program_uniforms[gl_state.program][location].uniform2i[0] = v0;
      program_uniforms[gl_state.program][location].uniform2i[1] = v1;
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform2fv.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform2f[0] != value[0] || program_uniforms[gl_state.program][location].uniform2f[1] != value[1]) {
      glUniform2fv(location, count, value);
      program_uniforms[gl_state.program][location].uniform2f[0] = value[0];
      program_uniforms[gl_state.program][location].uniform2f[1] = value[1];
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform3f.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform3f[0] != v0 || program_uniforms[gl_state.program][location].uniform3f[1] != v1 || program_uniforms[gl_state.program][location].uniform3f[2] != v2) {
      glUniform3f(location, v0, v1, v2);
      program_uniforms[gl_state.program][location].uniform3f[0] = v0;
      program_uniforms[gl_state.program][location].uniform3f[1] = v1;
      program_uniforms[gl_state.program][location].uniform3f[2] = v2;
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform3fv.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform3f[0] != value[0] || program_uniforms[gl_state.program][location].uniform3f[1] != value[1] || program_uniforms[gl_state.program][location].uniform3f[2] != value[2]) {
      glUniform3fv(location, count, value);
      program_uniforms[gl_state.program][location].uniform3f[0] = value[0];
      program_uniforms[gl_state.program][location].uniform3f[1] = value[1];
      program_uniforms[gl_state.program][location].uniform3f[2] = value[2];
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform4i.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform4i[0] != v0 || program_uniforms[gl_state.program][location].uniform4i[1] != v1 || program_uniforms[gl_state.program][location].uniform4i[2] != v2 || program_uniforms[gl_state.program][location].uniform4i[3] != v3) {
      glUniform4i(location, v0, v1, v2, v3);
      program_uniforms[gl_state.program][location].uniform4i[0] = v0;
      program_uniforms[gl_state.program][location].uniform4i[1] = v1;
      program_uniforms[gl_state.program][location].uniform4i[2] = v2;
      program_uniforms[gl_state.program][location].uniform4i[3] = v3;
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform4f.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform4f[0] != v0 || program_uniforms[gl_state.program][location].uniform4f[1] != v1 || program_uniforms[gl_state.program][location].uniform4f[2] != v2 || program_uniforms[gl_state.program][location].uniform4f[3] != v3) {
      glUniform4f(location, v0, v1, v2, v3);
      program_uniforms[gl_state.program][location].uniform4f[0] = v0;
      program_uniforms[gl_state.program][location].uniform4f[1] = v1;
      program_uniforms[gl_state.program][location].uniform4f[2] = v2;
      program_uniforms[gl_state.program][location].uniform4f[3] = v3;
   }
}

/*
 * Category: Shaders
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglUniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform4fv.\n");
#endif
   if (program_uniforms[gl_state.program][location].uniform4f[0] != value[0] || program_uniforms[gl_state.program][location].uniform4f[1] != value[1] || program_uniforms[gl_state.program][location].uniform4f[2] != value[2] || program_uniforms[gl_state.program][location].uniform4f[3] != value[3]) {
      glUniform4fv(location, count, value);
      program_uniforms[gl_state.program][location].uniform4f[0] = value[0];
      program_uniforms[gl_state.program][location].uniform4f[1] = value[1];
      program_uniforms[gl_state.program][location].uniform4f[2] = value[2];
      program_uniforms[gl_state.program][location].uniform4f[3] = value[3];
   }
}

/*
 *
 * Core in:
 * OpenGL    : 1.0
 */
void rglPolygonOffset(GLfloat factor, GLfloat units)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glPolygonOffset.\n");
#endif
   glsm_ctl(GLSM_CTL_IMM_VBO_DRAW, NULL);
   glPolygonOffset(factor, units);
   gl_state.polygonoffset.used   = true;
   gl_state.polygonoffset.factor = factor;
   gl_state.polygonoffset.units  = units;
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 3.0
 */
void rglGenFramebuffers(GLsizei n, GLuint *ids)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGenFramebuffers.\n");
#endif
   glGenFramebuffers(n, ids);
   int i;
   for (i = 0; i < n; ++i) {
      if (ids[i] < MAX_FRAMEBUFFERS)
         framebuffers[ids[i]] = (struct gl_framebuffers*)calloc(1, sizeof(struct gl_framebuffers));
   }
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 3.0
 */
void rglBindFramebuffer(GLenum target, GLuint framebuffer)
{
   if (framebuffer == 0)
      framebuffer = default_framebuffer;

   if (target == GL_FRAMEBUFFER) {
         gl_state.framebuf[0].desired_location = framebuffer;
         gl_state.framebuf[1].desired_location = framebuffer;
   }
#ifndef HAVE_OPENGLES2
   else if (target == GL_DRAW_FRAMEBUFFER) {
         gl_state.framebuf[0].desired_location = framebuffer;
   }
   else if (target == GL_READ_FRAMEBUFFER) {
         gl_state.framebuf[1].desired_location = framebuffer;
   }
#endif
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 3.0
 */
void rglDrawBuffers(GLsizei n, const GLenum *bufs)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDrawBuffers.\n");
#endif
#ifndef HAVE_OPENGLES2
   if (gl_state.framebuf[0].desired_location < MAX_FRAMEBUFFERS) {
      if (framebuffers[gl_state.framebuf[0].desired_location]->draw_buf_set == 0)
      {
         bindFBO(GL_DRAW_FRAMEBUFFER);
         glDrawBuffers(n, bufs);
         framebuffers[gl_state.framebuf[0].location]->draw_buf_set = 1;
      }
   } else {
      bindFBO(GL_DRAW_FRAMEBUFFER);
      glDrawBuffers(n, bufs);
   }
#endif
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 3.0
 */
void *rglMapBufferRange( 	GLenum target,
  	GLintptr offset,
  	GLsizeiptr length,
  	GLbitfield access)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glMapBufferRange.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   return glMapBufferRange(target, offset, length, access);
#else
   printf("WARNING! Not implemented.\n");
   return NULL;
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 4.3
 * OpenGLES  : 3.1
 */
void rglTexStorage2DMultisample(GLenum target, GLsizei samples,
      GLenum internalformat, GLsizei width, GLsizei height,
      GLboolean fixedsamplelocations)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glTexStorage2DMultisample.\n");
#endif
#ifndef HAVE_OPENGLES
   glTexStorage2DMultisample(target, samples, internalformat,
         width, height, fixedsamplelocations);
#else
   m_glTexStorage2DMultisample(target, samples, internalformat,
         width, height, fixedsamplelocations);
#endif
}

/*
 *
 * Core in:
 * OpenGLES  : 3.0
 */
void rglTexStorage2D(GLenum target, GLsizei levels, GLenum internalFormat,
      GLsizei width, GLsizei height)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glTexStorage2D.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glTexStorage2D(target, levels, internalFormat, width, height);
#endif
}
/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : 3.2
 */
void rglDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex)
{
#ifdef HAVE_OPENGLES
   bindFBO(GL_FRAMEBUFFER);
   m_glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
#else
   bindFBO(GL_DRAW_FRAMEBUFFER);
   glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 4.2
 * OpenGLES  : 3.1
 */
void rglMemoryBarrier( 	GLbitfield barriers)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glMemoryBarrier.\n");
#endif
#ifndef HAVE_OPENGLES
   glMemoryBarrier(barriers);
#else
   m_glMemoryBarrier(barriers);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 4.2
 * OpenGLES  : 3.1
 */
void rglBindImageTexture( 	GLuint unit,
  	GLuint texture,
  	GLint level,
  	GLboolean layered,
  	GLint layer,
  	GLenum access,
  	GLenum format)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBindImageTexture.\n");
#endif
#ifndef HAVE_OPENGLES
   glBindImageTexture(unit, texture, level, layered, layer, access, format);
#else
   m_glBindImageTexture(unit, texture, level, layered, layer, access, format);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 4.1
 * OpenGLES  : 3.1
 */
void rglGetProgramBinary( 	GLuint program,
  	GLsizei bufsize,
  	GLsizei *length,
  	GLenum *binaryFormat,
  	void *binary)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGetProgramBinary.\n");
#endif
#if !defined(HAVE_OPENGLES) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGetProgramBinary(program, bufsize, length, binaryFormat, binary);
#else
   printf("WARNING! Not implemented.\n");
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 4.1
 * OpenGLES  : 3.1
 */
void rglProgramBinary(GLuint program,
  	GLenum binaryFormat,
  	const void *binary,
  	GLsizei length)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glProgramBinary.\n");
#endif
#if !defined(HAVE_OPENGLES) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES_3_1)
   glProgramBinary(program, binaryFormat, binary, length);
#else
   printf("WARNING! Not implemented.\n");
#endif
}

void rglTexImage2DMultisample( 	GLenum target,
  	GLsizei samples,
  	GLenum internalformat,
  	GLsizei width,
  	GLsizei height,
  	GLboolean fixedsamplelocations)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glTexImage2DMultisample.\n");
#endif
#ifndef HAVE_OPENGLES
   glTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
#endif
}

void rglTexImage3D(	GLenum target,
 	GLint level,
 	GLint internalFormat,
 	GLsizei width,
 	GLsizei height,
 	GLsizei depth,
 	GLint border,
 	GLenum format,
 	GLenum type,
 	const GLvoid * data)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glTexImage3D.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glTexImage3D(target, level, internalFormat, width, height, depth, border, format, type, data);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 1.5
 */
void * rglMapBuffer(	GLenum target, GLenum access)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glMapBuffer.\n");
#endif
#if defined(HAVE_OPENGLES)
   return glMapBufferOES(target, access);
#else
   return glMapBuffer(target, access);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 1.5
 */
GLboolean rglUnmapBuffer( 	GLenum target)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUnmapBuffer.\n");
#endif
#if defined(HAVE_OPENGLES)
   return glUnmapBufferOES(target);
#else
   return glUnmapBuffer(target);
#endif
}

void rglBlendEquation(GLenum mode)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBlendEquation.\n");
#endif
   glBlendEquation(mode);
}

void rglBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBlendColor.\n");
#endif
   glBlendColor(red, green, blue, alpha);
}

/*
 * Category: Blending
 *
 * Core in:
 * OpenGL    : 2.0
 */
void rglBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBlendEquationSeparate.\n");
#endif
   glBlendEquationSeparate(modeRGB, modeAlpha);
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 3.2
 */
void rglCopyImageSubData( 	GLuint srcName,
  	GLenum srcTarget,
  	GLint srcLevel,
  	GLint srcX,
  	GLint srcY,
  	GLint srcZ,
  	GLuint dstName,
  	GLenum dstTarget,
  	GLint dstLevel,
  	GLint dstX,
  	GLint dstY,
  	GLint dstZ,
  	GLsizei srcWidth,
  	GLsizei srcHeight,
  	GLsizei srcDepth)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glCopyImageSubData.\n");
#endif
#ifndef HAVE_OPENGLES
   glCopyImageSubData(srcName,
         srcTarget,
         srcLevel,
         srcX,
         srcY,
         srcZ,
         dstName,
         dstTarget,
         dstLevel,
         dstX,
         dstY,
         dstZ,
         srcWidth,
         srcHeight,
         srcDepth);
#else
   m_glCopyImageSubData(srcName,
         srcTarget,
         srcLevel,
         srcX,
         srcY,
         srcZ,
         dstName,
         dstTarget,
         dstLevel,
         dstX,
         dstY,
         dstZ,
         srcWidth,
         srcHeight,
         srcDepth);
#endif
}

/*
 * Category: FBO
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 3.0
 */
void rglBlitFramebuffer(
      GLint srcX0, GLint srcY0,
      GLint srcX1, GLint srcY1,
      GLint dstX0, GLint dstY0,
      GLint dstX1, GLint dstY1,
      GLbitfield mask, GLenum filter)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBlitFramebuffer.\n");
#endif
#ifndef HAVE_OPENGLES2
   GLuint src_attachment;
   GLuint dst_attachment;
   const bool good_pointer = gl_state.framebuf[0].desired_location < MAX_FRAMEBUFFERS && gl_state.framebuf[1].desired_location < MAX_FRAMEBUFFERS;
   const bool good_target = framebuffers[gl_state.framebuf[0].desired_location]->target == framebuffers[gl_state.framebuf[1].desired_location]->target;
   const bool sameSize = dstX1 - dstX0 == srcX1 - srcX0 && dstY1 - dstY0 == srcY1 - srcY0;
   if (sameSize && copy_image_support && good_pointer && good_target) {
      if (mask == GL_COLOR_BUFFER_BIT) {
         src_attachment = framebuffers[gl_state.framebuf[1].desired_location]->color_attachment;
         dst_attachment = framebuffers[gl_state.framebuf[0].desired_location]->color_attachment;
      } else if (mask == GL_DEPTH_BUFFER_BIT) {
         src_attachment = framebuffers[gl_state.framebuf[1].desired_location]->depth_attachment;
         dst_attachment = framebuffers[gl_state.framebuf[0].desired_location]->depth_attachment;
      }
      rglCopyImageSubData(src_attachment, framebuffers[gl_state.framebuf[1].desired_location]->target, 0, srcX0, srcY0, 0,
         dst_attachment, framebuffers[gl_state.framebuf[0].desired_location]->target, 0, dstX0, dstY0, 0, srcX1 - srcX0, srcY1 - srcY0, 1);
   } else {
      bindFBO(GL_DRAW_FRAMEBUFFER);
      bindFBO(GL_READ_FRAMEBUFFER);
      glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1,
         dstX0, dstY0, dstX1, dstY1,
         mask, filter);
   }
#endif
}

/*
 * Category: VAO
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 3.0
 */
void rglBindVertexArray(GLuint array)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBindVertexArray.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   bindFBO(GL_FRAMEBUFFER);
   gl_state.bindvertex.array = array;
   glBindVertexArray(array);
#endif
}

/*
 * Category: VAO
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 3.0
 */
void rglGenVertexArrays(GLsizei n, GLuint *arrays)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glGenVertexArrays.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glGenVertexArrays(n, arrays);
#endif
}

/*
 * Category: VAO
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 3.0
 */
void rglDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteVertexArrays.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glDeleteVertexArrays(n, arrays);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : 3.0
 */
void *rglFenceSync(GLenum condition, GLbitfield flags)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glFenceSync.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   return (GLsync)glFenceSync(condition, flags);
#else
   return NULL;
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : 3.0
 */
void rglDeleteSync(GLsync sync)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDeleteSync.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
  glDeleteSync((GLsync)sync);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : 3.0
 */
void rglWaitSync(void *sync, GLbitfield flags, uint64_t timeout)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glWaitSync.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glWaitSync((GLsync)sync, flags, (GLuint64)timeout);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 4.4
 * OpenGLES  : Not available
 */
void rglBufferStorage(GLenum target, GLsizeiptr size, const GLvoid *data, GLbitfield flags)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glBufferStorage.\n");
#endif
#ifndef HAVE_OPENGLES
   glBufferStorage(target, size, data, flags);
#else
   m_glBufferStorage(target, size, data, flags);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 2.0
 * OpenGLES  : 2.0
 */

void rglUniform2iv(	GLint location,
 	GLsizei count,
 	const GLint *value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform2iv.\n");
#endif
   glUniform2iv(location, count, value);
}

/*
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : ?.?
 */

void rglUniform2uiv(	GLint location,
 	GLsizei count,
 	const GLuint *value)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glUniform2uiv.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
   glUniform2uiv(location, count, value);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 4.3
 * OpenGLES  : ?.?
 */
void rglTextureView(	GLuint texture,
 	GLenum target,
 	GLuint origtexture,
 	GLenum internalformat,
 	GLuint minlevel,
 	GLuint numlevels,
 	GLuint minlayer,
 	GLuint numlayers)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glTextureView.\n");
#endif
#if defined(HAVE_OPENGL)
   glTextureView(texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 3.0
 * OpenGLES  : 3.0
 */
void rglFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glFlushMappedBufferRange.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
  glFlushMappedBufferRange(target, offset, length);
#endif
}

#ifndef GL_WAIT_FAILED
#define GL_WAIT_FAILED                                   0x911D
#endif

/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : 3.0
 */
GLenum rglClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glClientWaitSync.\n");
#endif
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) && defined(HAVE_OPENGLES3)
  return glClientWaitSync((GLsync)sync, flags, (GLuint64)timeout);
#else
  return GL_WAIT_FAILED;
#endif
}

/*
 *
 * Core in:
 * OpenGL    : 3.2
 * OpenGLES  : Not available
 */
void rglDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type,
			       GLvoid *indices, GLint basevertex)
{
#ifdef GLSM_DEBUG
   log_cb(RETRO_LOG_INFO, "glDrawElementsBaseVertex.\n");
#endif
#if defined(HAVE_OPENGL)
   bindFBO(GL_DRAW_FRAMEBUFFER);
   glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
#endif
}

/* GLSM-side */

bool isExtensionSupported(const char *extension)
{
#ifdef GL_NUM_EXTENSIONS
	GLint count = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &count);
	for (int i = 0; i < count; ++i) {
		const char* name = (const char*)glGetStringi(GL_EXTENSIONS, i);
		if (name == NULL)
			continue;
		if (strcmp(extension, name) == 0)
			return true;
	}
	return false;
#else
	GLubyte *where = (GLubyte *)strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;

	const GLubyte *extensions = glGetString(GL_EXTENSIONS);

	const GLubyte *start = extensions;
	for (;;) {
		where = (GLubyte *)strstr((const char *)start, extension);
		if (where == NULL)
			break;

		GLubyte *terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
		if (*terminator == ' ' || *terminator == '\0')
			return true;

		start = terminator;
	}

	return false;
#endif // GL_NUM_EXTENSIONS
}

static void glsm_state_setup(void)
{
   GLint majorVersion = 0;
   GLint minorVersion = 0;
   bool copy_image_support_version = 0;
#ifndef HAVE_OPENGLES2
   glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
   glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
#endif
#ifdef HAVE_OPENGLES
   if (majorVersion >= 3 && minorVersion >= 2)
      copy_image_support_version = 1;
#else
   if (majorVersion >= 4 && minorVersion >= 3)
      copy_image_support_version = 1;
#endif
   copy_image_support = isExtensionSupported("GL_ARB_copy_image") || isExtensionSupported("GL_EXT_copy_image") || copy_image_support_version;
#ifdef HAVE_OPENGLES
   m_glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)glsm_get_proc_address("glDrawRangeElementsBaseVertex");
   m_glBufferStorage = (PFNGLBUFFERSTORAGEEXTPROC)glsm_get_proc_address("glBufferStorageEXT");
   m_glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)glsm_get_proc_address("glMemoryBarrier");
   m_glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)glsm_get_proc_address("glBindImageTexture");
   m_glTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)glsm_get_proc_address("glTexStorage2DMultisample");
   m_glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)glsm_get_proc_address("glCopyImageSubData");
   if (m_glCopyImageSubData == NULL)
      m_glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)glsm_get_proc_address("glCopyImageSubDataEXT");
#endif

   unsigned i;

   memset(&gl_state, 0, sizeof(struct gl_cached_state));
   gl_state.cap_translate[SGL_DEPTH_TEST]               = GL_DEPTH_TEST;
   gl_state.cap_translate[SGL_BLEND]                    = GL_BLEND;
   gl_state.cap_translate[SGL_POLYGON_OFFSET_FILL]      = GL_POLYGON_OFFSET_FILL;
   gl_state.cap_translate[SGL_FOG]                      = GL_FOG;
   gl_state.cap_translate[SGL_CULL_FACE]                = GL_CULL_FACE;
   gl_state.cap_translate[SGL_ALPHA_TEST]               = GL_ALPHA_TEST;
   gl_state.cap_translate[SGL_SCISSOR_TEST]             = GL_SCISSOR_TEST;
   gl_state.cap_translate[SGL_STENCIL_TEST]             = GL_STENCIL_TEST;
   gl_state.cap_translate[SGL_DITHER]                   = GL_DITHER;
   gl_state.cap_translate[SGL_SAMPLE_ALPHA_TO_COVERAGE] = GL_SAMPLE_ALPHA_TO_COVERAGE;
   gl_state.cap_translate[SGL_SAMPLE_COVERAGE]          = GL_SAMPLE_COVERAGE;
#ifndef HAVE_OPENGLES
   gl_state.cap_translate[SGL_COLOR_LOGIC_OP]       = GL_COLOR_LOGIC_OP;
   gl_state.cap_translate[SGL_CLIP_DISTANCE0]       = GL_CLIP_DISTANCE0;
   gl_state.cap_translate[SGL_DEPTH_CLAMP]          = GL_DEPTH_CLAMP;
#endif

   for (i = 0; i < MAX_ATTRIB; i++)
   {
      gl_state.vertex_attrib_pointer.enabled[i] = 0;
      gl_state.attrib_pointer.used[i] = 0;
   }

   gl_state.bindvertex.array = 0;
   glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &glsm_max_textures);
   if(glsm_max_textures > 32)
      glsm_max_textures = 32;

   for (i = 0; i < glsm_max_textures; ++i) {
      gl_state.bind_textures.target[i] = GL_TEXTURE_2D;
      gl_state.bind_textures.ids[i] = 0;
   }

   gl_state.pixelstore.pack             = 4;
   gl_state.pixelstore.unpack           = 4;
   gl_state.array_buffer                = 0;
   gl_state.index_buffer                = 0;

   default_framebuffer                  = glsm_get_current_framebuffer();

   gl_state.framebuf[0].location        = default_framebuffer;
   gl_state.framebuf[1].location        = default_framebuffer;
   gl_state.framebuf[0].desired_location        = default_framebuffer;
   gl_state.framebuf[1].desired_location        = default_framebuffer;

   glBindFramebuffer(GL_FRAMEBUFFER, default_framebuffer);
   GLint params;
   if (!resetting_context)
      framebuffers[default_framebuffer] = (struct gl_framebuffers*)calloc(1, sizeof(struct gl_framebuffers));

   glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &params);
   framebuffers[default_framebuffer]->color_attachment = params;
   glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &params);
   framebuffers[default_framebuffer]->depth_attachment = params;
   framebuffers[default_framebuffer]->target = GL_TEXTURE_2D;

   gl_state.cullface.mode               = GL_BACK;
   gl_state.frontface.mode              = GL_CCW;

   gl_state.blendfunc_separate.used     = false;
   gl_state.blendfunc_separate.srcRGB   = GL_ONE;
   gl_state.blendfunc_separate.dstRGB   = GL_ZERO;
   gl_state.blendfunc_separate.srcAlpha = GL_ONE;
   gl_state.blendfunc_separate.dstAlpha = GL_ZERO;

   gl_state.depthfunc.used              = false;

   gl_state.colormask.used              = false;
   gl_state.colormask.red               = GL_TRUE;
   gl_state.colormask.green             = GL_TRUE;
   gl_state.colormask.blue              = GL_TRUE;
   gl_state.colormask.alpha             = GL_TRUE;

   gl_state.polygonoffset.used          = false;

   gl_state.depthfunc.func              = GL_LESS;

#ifndef HAVE_OPENGLES
   gl_state.colorlogicop                = GL_COPY;
#endif

#ifdef CORE
   glGenVertexArrays(1, &gl_state.vao);
#endif
}

static void glsm_state_bind(void)
{
   unsigned i;
#ifndef HAVE_OPENGLES2
   if (gl_state.bindvertex.array != 0) {
      glBindVertexArray(gl_state.bindvertex.array);
      gl_state.array_buffer = 0;
   } else
#endif
   {
      for (i = 0; i < MAX_ATTRIB; i++)
      {
         if (gl_state.vertex_attrib_pointer.enabled[i])
            glEnableVertexAttribArray(i);

         if (gl_state.attrib_pointer.used[i]) {
            glVertexAttribPointer(
               i,
               gl_state.attrib_pointer.size[i],
               gl_state.attrib_pointer.type[i],
               gl_state.attrib_pointer.normalized[i],
               gl_state.attrib_pointer.stride[i],
               gl_state.attrib_pointer.pointer[i]);
         }
      }
    }
    
   glPixelStorei(GL_UNPACK_ALIGNMENT, gl_state.pixelstore.unpack);
   glPixelStorei(GL_PACK_ALIGNMENT, gl_state.pixelstore.pack);

   if (EnableFBEmulation) {
      gl_state.framebuf[0].location = 0;
      gl_state.framebuf[1].location = 0;
   } else {
      glBindFramebuffer(GL_FRAMEBUFFER, default_framebuffer);
      gl_state.framebuf[0].location = default_framebuffer;
      gl_state.framebuf[1].location = default_framebuffer;
   }

   for(i = 0; i < SGL_CAP_MAX; i ++)
   {
      if (gl_state.cap_state[i])
         glEnable(gl_state.cap_translate[i]);
   }
   
   if (gl_state.blendfunc.used)
      glBlendFunc(
            gl_state.blendfunc.sfactor,
            gl_state.blendfunc.dfactor);

   if (gl_state.blendfunc_separate.used)
      glBlendFuncSeparate(
            gl_state.blendfunc_separate.srcRGB,
            gl_state.blendfunc_separate.dstRGB,
            gl_state.blendfunc_separate.srcAlpha,
            gl_state.blendfunc_separate.dstAlpha
            );

   glClearColor(
         gl_state.clear_color.r,
         gl_state.clear_color.g,
         gl_state.clear_color.b,
         gl_state.clear_color.a);

   if (gl_state.depthfunc.used)
      glDepthFunc(gl_state.depthfunc.func);

   if (gl_state.colormask.used)
      glColorMask(
            gl_state.colormask.red,
            gl_state.colormask.green,
            gl_state.colormask.blue,
            gl_state.colormask.alpha);

   if (gl_state.cullface.used)
      glCullFace(gl_state.cullface.mode);

   if (gl_state.depthmask.used)
      glDepthMask(gl_state.depthmask.mask);

   if (gl_state.polygonoffset.used)
      glPolygonOffset(
            gl_state.polygonoffset.factor,
            gl_state.polygonoffset.units);

   if (gl_state.scissor.used)
      glScissor(
            gl_state.scissor.x,
            gl_state.scissor.y,
            gl_state.scissor.w,
            gl_state.scissor.h);

   glUseProgram(gl_state.program);

   glViewport(
         gl_state.viewport.x,
         gl_state.viewport.y,
         gl_state.viewport.w,
         gl_state.viewport.h);

   if (gl_state.frontface.used)
      glFrontFace(gl_state.frontface.mode);

   if (gl_state.stencilmask.used)
      glStencilMask(gl_state.stencilmask.mask);

   if (gl_state.stencilop.used)
      glStencilOp(gl_state.stencilop.sfail,
            gl_state.stencilop.dpfail,
            gl_state.stencilop.dppass);

   if (gl_state.stencilfunc.used)
      glStencilFunc(
            gl_state.stencilfunc.func,
            gl_state.stencilfunc.ref,
            gl_state.stencilfunc.mask);

   glActiveTexture(GL_TEXTURE0 + active_texture);
   glBindTexture(gl_state.bind_textures.target[active_texture], gl_state.bind_textures.ids[active_texture]);
}

static void glsm_state_unbind(void)
{
   unsigned i;
   for (i = 0; i < SGL_CAP_MAX; i ++)
   {
      if (gl_state.cap_state[i])
         glDisable(gl_state.cap_translate[i]);
   }

   glBlendFunc(GL_ONE, GL_ZERO);

   if (gl_state.colormask.used)
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      
   if (gl_state.blendfunc_separate.used)
      glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);

   if (gl_state.cullface.used)
      glCullFace(GL_BACK);

   if (gl_state.depthmask.used)
      glDepthMask(GL_TRUE);

   if (gl_state.polygonoffset.used)
      glPolygonOffset(0, 0);

   glUseProgram(0);
   glClearColor(0,0,0,0.0f);

   if (gl_state.depthrange.used)
      rglDepthRange(0, 1);

   glStencilMask(1);
   glFrontFace(GL_CCW);
   
   if (gl_state.depthfunc.used)
      glDepthFunc(GL_LESS);

   if (gl_state.stencilop.used)
      glStencilOp(GL_KEEP,GL_KEEP, GL_KEEP);

   if (gl_state.stencilfunc.used)
      glStencilFunc(GL_ALWAYS,0,1);

   glActiveTexture(GL_TEXTURE0);


#ifndef HAVE_OPENGLES2
   if (gl_state.bindvertex.array != 0)
      glBindVertexArray(0);
   else
#endif
   {
      for (i = 0; i < MAX_ATTRIB; i++)
      {
         if (gl_state.vertex_attrib_pointer.enabled[i])
            glDisableVertexAttribArray(i);
      }
    }

   glBindFramebuffer(RARCH_GL_FRAMEBUFFER, 0);
}

static bool glsm_state_ctx_destroy(void *data)
{
   return true;
}

static bool glsm_state_ctx_init(glsm_ctx_params_t *params)
{
   if (!params || !params->environ_cb)
      return false;

#ifdef HAVE_OPENGLES
#if defined(HAVE_OPENGLES_3_1)
   hw_render.context_type       = RETRO_HW_CONTEXT_OPENGLES_VERSION;
   hw_render.version_major      = 3;
   hw_render.version_minor      = 1;
#elif defined(HAVE_OPENGLES3)
   hw_render.context_type       = RETRO_HW_CONTEXT_OPENGLES3;
#else
   hw_render.context_type       = RETRO_HW_CONTEXT_OPENGLES2;
#endif
#else
#if defined(CORE) && !defined(HAVE_LIBNX)
   hw_render.context_type       = RETRO_HW_CONTEXT_OPENGL_CORE;
   hw_render.version_major      = 3;
   hw_render.version_minor      = 3;
#else
   hw_render.context_type       = RETRO_HW_CONTEXT_OPENGL;
   if (params->major != 0)
      hw_render.version_major   = params->major;
   if (params->minor != 0)
      hw_render.version_minor   = params->minor;
#endif // defined(CORE) && !defined(HAVE_LIBNX)
#endif

   hw_render.context_reset      = params->context_reset;
   hw_render.context_destroy    = params->context_destroy;
   hw_render.stencil            = params->stencil;
   hw_render.depth              = true;
   hw_render.bottom_left_origin = true;
   hw_render.cache_context      = true;

   if (!params->environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render))
      return false;

   return true;
}

GLuint glsm_get_current_framebuffer(void)
{
   return hw_render.get_current_framebuffer();
}

void* glsm_get_proc_address(const char* sym)
{
   if (!hw_render.get_proc_address)
      return NULL;
   
   return hw_render.get_proc_address(sym);
}

extern void initGLFunctions();
bool glsm_ctl(enum glsm_state_ctl state, void *data)
{
   switch (state)
   {
      case GLSM_CTL_IMM_VBO_DRAW:
         return false;
      case GLSM_CTL_IMM_VBO_DISABLE:
         return false;
      case GLSM_CTL_IS_IMM_VBO:
         return false;
      case GLSM_CTL_SET_IMM_VBO:
         break;
      case GLSM_CTL_UNSET_IMM_VBO:
         break;
      case GLSM_CTL_PROC_ADDRESS_GET:
         {
            glsm_ctx_proc_address_t *proc = (glsm_ctx_proc_address_t*)data;
            if (!hw_render.get_proc_address)
               return false;
            proc->addr = hw_render.get_proc_address;
         }
         break;
      case GLSM_CTL_STATE_CONTEXT_RESET:
         rglgen_resolve_symbols(hw_render.get_proc_address);
         initGLFunctions();
         
         if (window_first > 0) {
            resetting_context = 1;
            glsm_state_setup();
            retroChangeWindow();
            glsm_state_unbind();
            resetting_context = 0;
	      }
         else
            window_first = 1; 
         break;
      case GLSM_CTL_STATE_CONTEXT_DESTROY:
         glsm_state_ctx_destroy(data);
         break;
      case GLSM_CTL_STATE_CONTEXT_INIT:
         return glsm_state_ctx_init((glsm_ctx_params_t*)data);
      case GLSM_CTL_STATE_SETUP:
         glsm_state_setup();
         break;
      case GLSM_CTL_STATE_UNBIND:
         glsm_state_unbind();
         break;
      case GLSM_CTL_STATE_BIND:
         glsm_state_bind();
         break;
      case GLSM_CTL_NONE:
      default:
         break;
   }

   return true;
}
