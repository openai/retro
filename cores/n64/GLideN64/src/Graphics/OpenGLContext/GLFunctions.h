#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#ifdef OS_WINDOWS
#include <windows.h>
#elif defined(OS_LINUX)
#include <winlnxdefs.h>
#endif

#ifdef GL_GLEXT_PROTOTYPES
#undef GL_GLEXT_PROTOTYPES
#endif // GL_GLEXT_PROTOTYPES

#ifdef EGL
#include <GL/glcorearb.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#elif defined(OS_MAC_OS_X)
#include <OpenGL/OpenGL.h>
#include <stddef.h>
#include <OpenGL/gl3.h>

#elif defined(OS_IOS)
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
// Add missing type defintions for iOS
typedef double GLclampd;
typedef double GLdouble;
// These will get redefined by other GL headers.
#undef GL_DRAW_FRAMEBUFFER_BINDING
#undef GL_COPY_READ_BUFFER_BINDING
#undef GL_COPY_WRITE_BUFFER_BINDING
#include <GL/glcorearb.h>
#else
#include <GL/gl.h>
#include <GL/glcorearb.h>
#endif

#define GL_LUMINANCE 0x1909
#include <GL/glext.h>
#include <stdexcept>
#include <sstream>
#include "Log.h"

#define IS_GL_FUNCTION_VALID(proc_name) ptr##proc_name != nullptr

#if !defined(EGL) && !defined(OS_IOS)
typedef void (APIENTRYP PFNGLPOLYGONOFFSETPROC) (GLfloat factor, GLfloat units);
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (APIENTRYP PFNGLTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (APIENTRYP PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
#endif

extern PFNGLBLENDFUNCPROC ptrBlendFunc;
extern PFNGLPIXELSTOREIPROC ptrPixelStorei;
extern PFNGLCLEARCOLORPROC ptrClearColor;
extern PFNGLCULLFACEPROC ptrCullFace;
extern PFNGLDEPTHFUNCPROC ptrDepthFunc;
extern PFNGLDEPTHMASKPROC ptrDepthMask;
extern PFNGLDISABLEPROC ptrDisable;
extern PFNGLENABLEPROC ptrEnable;
extern PFNGLPOLYGONOFFSETPROC ptrPolygonOffset;
extern PFNGLSCISSORPROC ptrScissor;
extern PFNGLVIEWPORTPROC ptrViewport;
extern PFNGLBINDTEXTUREPROC ptrBindTexture;
extern PFNGLTEXIMAGE2DPROC ptrTexImage2D;
extern PFNGLTEXPARAMETERIPROC ptrTexParameteri;
extern PFNGLGETINTEGERVPROC ptrGetIntegerv;
extern PFNGLGETSTRINGPROC ptrGetString;
extern PFNGLREADPIXELSPROC ptrReadPixels;
extern PFNGLTEXSUBIMAGE2DPROC ptrTexSubImage2D;
extern PFNGLDRAWARRAYSPROC ptrDrawArrays;
extern PFNGLGETERRORPROC ptrGetError;
extern PFNGLDRAWELEMENTSPROC ptrDrawElements;
extern PFNGLLINEWIDTHPROC ptrLineWidth;
extern PFNGLCLEARPROC ptrClear;
extern PFNGLGETFLOATVPROC ptrGetFloatv;
extern PFNGLDELETETEXTURESPROC ptrDeleteTextures;
extern PFNGLGENTEXTURESPROC ptrGenTextures;
extern PFNGLTEXPARAMETERFPROC ptrTexParameterf;
extern PFNGLACTIVETEXTUREPROC ptrActiveTexture;
extern PFNGLBLENDCOLORPROC ptrBlendColor;
extern PFNGLREADBUFFERPROC ptrReadBuffer;
extern PFNGLFINISHPROC ptrFinish;
#if defined(OS_ANDROID)
struct AHardwareBuffer;
typedef EGLClientBuffer (EGLAPIENTRYP PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC) (const struct AHardwareBuffer *buffer);
extern PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC ptrGetNativeClientBufferANDROID;
#endif

#ifdef OS_WINDOWS
extern PFNGLACTIVETEXTUREPROC ptrActiveTexture;
extern PFNGLBLENDCOLORPROC ptrBlendColor;
#endif

extern PFNGLCREATESHADERPROC ptrCreateShader;
extern PFNGLCOMPILESHADERPROC ptrCompileShader;
extern PFNGLSHADERSOURCEPROC ptrShaderSource;
extern PFNGLCREATEPROGRAMPROC ptrCreateProgram;
extern PFNGLATTACHSHADERPROC ptrAttachShader;
extern PFNGLLINKPROGRAMPROC ptrLinkProgram;
extern PFNGLUSEPROGRAMPROC ptrUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC ptrGetUniformLocation;
extern PFNGLUNIFORM1IPROC ptrUniform1i;
extern PFNGLUNIFORM1FPROC ptrUniform1f;
extern PFNGLUNIFORM2FPROC ptrUniform2f;
extern PFNGLUNIFORM2IPROC ptrUniform2i;
extern PFNGLUNIFORM4IPROC ptrUniform4i;

extern PFNGLUNIFORM4FPROC ptrUniform4f;
extern PFNGLUNIFORM3FVPROC ptrUniform3fv;
extern PFNGLUNIFORM4FVPROC ptrUniform4fv;
extern PFNGLDETACHSHADERPROC ptrDetachShader;
extern PFNGLDELETESHADERPROC ptrDeleteShader;
extern PFNGLDELETEPROGRAMPROC ptrDeleteProgram;
extern PFNGLGETPROGRAMINFOLOGPROC ptrGetProgramInfoLog;
extern PFNGLGETSHADERINFOLOGPROC ptrGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC ptrGetShaderiv;
extern PFNGLGETPROGRAMIVPROC ptrGetProgramiv;

extern PFNGLENABLEVERTEXATTRIBARRAYPROC ptrEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC ptrDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC ptrVertexAttribPointer;
extern PFNGLBINDATTRIBLOCATIONPROC ptrBindAttribLocation;
extern PFNGLVERTEXATTRIB1FPROC ptrVertexAttrib1f;
extern PFNGLVERTEXATTRIB4FPROC ptrVertexAttrib4f;
extern PFNGLVERTEXATTRIB4FVPROC ptrVertexAttrib4fv;

extern PFNGLDEPTHRANGEFPROC ptrDepthRangef;
extern PFNGLCLEARDEPTHFPROC ptrClearDepthf;

extern PFNGLDRAWBUFFERSPROC ptrDrawBuffers;
extern PFNGLGENFRAMEBUFFERSPROC ptrGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC ptrBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC ptrDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC ptrFramebufferTexture2D;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC ptrTexImage2DMultisample;
extern PFNGLTEXSTORAGE2DMULTISAMPLEPROC ptrTexStorage2DMultisample;
extern PFNGLGENRENDERBUFFERSPROC ptrGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC ptrBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC ptrRenderbufferStorage;
extern PFNGLDELETERENDERBUFFERSPROC ptrDeleteRenderbuffers;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC ptrFramebufferRenderbuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC ptrCheckFramebufferStatus;
extern PFNGLBLITFRAMEBUFFERPROC ptrBlitFramebuffer;
extern PFNGLGENVERTEXARRAYSPROC ptrGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC ptrBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC ptrDeleteVertexArrays;
extern PFNGLGENBUFFERSPROC ptrGenBuffers;
extern PFNGLBINDBUFFERPROC ptrBindBuffer;
extern PFNGLBUFFERDATAPROC ptrBufferData;
extern PFNGLMAPBUFFERPROC ptrMapBuffer;
extern PFNGLMAPBUFFERRANGEPROC ptrMapBufferRange;
extern PFNGLUNMAPBUFFERPROC ptrUnmapBuffer;
extern PFNGLDELETEBUFFERSPROC ptrDeleteBuffers;
extern PFNGLBINDIMAGETEXTUREPROC ptrBindImageTexture;
extern PFNGLMEMORYBARRIERPROC ptrMemoryBarrier;
extern PFNGLGETSTRINGIPROC ptrGetStringi;
extern PFNGLINVALIDATEFRAMEBUFFERPROC ptrInvalidateFramebuffer;
extern PFNGLBUFFERSTORAGEPROC ptrBufferStorage;
extern PFNGLFENCESYNCPROC ptrFenceSync;
extern PFNGLCLIENTWAITSYNCPROC ptrClientWaitSync;
extern PFNGLDELETESYNCPROC ptrDeleteSync;

extern PFNGLGETUNIFORMBLOCKINDEXPROC ptrGetUniformBlockIndex;
extern PFNGLUNIFORMBLOCKBINDINGPROC ptrUniformBlockBinding;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC ptrGetActiveUniformBlockiv;
extern PFNGLGETUNIFORMINDICESPROC ptrGetUniformIndices;
extern PFNGLGETACTIVEUNIFORMSIVPROC ptrGetActiveUniformsiv;
extern PFNGLBINDBUFFERBASEPROC ptrBindBufferBase;
extern PFNGLBUFFERSUBDATAPROC ptrBufferSubData;

extern PFNGLGETPROGRAMBINARYPROC ptrGetProgramBinary;
extern PFNGLPROGRAMBINARYPROC ptrProgramBinary;
extern PFNGLPROGRAMPARAMETERIPROC ptrProgramParameteri;

extern PFNGLTEXSTORAGE2DPROC ptrTexStorage2D;
extern PFNGLTEXTURESTORAGE2DPROC ptrTextureStorage2D;
extern PFNGLTEXTURESUBIMAGE2DPROC ptrTextureSubImage2D;
extern PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC ptrTextureStorage2DMultisample;
extern PFNGLTEXTUREPARAMETERIPROC ptrTextureParameteri;
extern PFNGLTEXTUREPARAMETERFPROC ptrTextureParameterf;
extern PFNGLCREATETEXTURESPROC ptrCreateTextures;
extern PFNGLCREATEBUFFERSPROC ptrCreateBuffers;
extern PFNGLCREATEFRAMEBUFFERSPROC ptrCreateFramebuffers;
extern PFNGLNAMEDFRAMEBUFFERTEXTUREPROC ptrNamedFramebufferTexture;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC ptrDrawRangeElementsBaseVertex;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC ptrFlushMappedBufferRange;
extern PFNGLTEXTUREBARRIERPROC ptrTextureBarrier;
extern PFNGLTEXTUREBARRIERNVPROC ptrTextureBarrierNV;
extern PFNGLCLEARBUFFERFVPROC ptrClearBufferfv;
extern PFNGLENABLEIPROC ptrEnablei;
extern PFNGLDISABLEIPROC ptrDisablei;
extern PFNGLDEBUGMESSAGECALLBACKPROC ptrDebugMessageCallback;
extern PFNGLDEBUGMESSAGECONTROLPROC ptrDebugMessageControl;
extern PFNGLCOPYTEXIMAGE2DPROC ptrCopyTexImage2D;

typedef void (APIENTRYP PFNGLEGLIMAGETARGETTEXTURE2DOESPROC) (GLenum target, void* image);
extern PFNGLEGLIMAGETARGETTEXTURE2DOESPROC ptrEGLImageTargetTexture2DOES;

typedef void (APIENTRYP PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC) (GLenum target, void* image);
extern PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC ptrEGLImageTargetRenderbufferStorageOES;

extern "C" void initGLFunctions();

#ifndef NO_GL_WRAP
#define glGetError(...) opengl::FunctionWrapper::wrGetError(__VA_ARGS__)
#define glBlendFunc(...) opengl::FunctionWrapper::wrBlendFunc(__VA_ARGS__)
#define glPixelStorei(...) opengl::FunctionWrapper::wrPixelStorei(__VA_ARGS__)
#define glClearColor(...) opengl::FunctionWrapper::wrClearColor(__VA_ARGS__)
#define glCullFace(...) opengl::FunctionWrapper::wrCullFace(__VA_ARGS__)
#define glDepthFunc(...) opengl::FunctionWrapper::wrDepthFunc(__VA_ARGS__)
#define glDepthMask(...) opengl::FunctionWrapper::wrDepthMask(__VA_ARGS__)
#define glDisable(...) opengl::FunctionWrapper::wrDisable(__VA_ARGS__)
#define glEnable(...) opengl::FunctionWrapper::wrEnable(__VA_ARGS__)
#define glPolygonOffset(...) opengl::FunctionWrapper::wrPolygonOffset(__VA_ARGS__)
#define glScissor(...) opengl::FunctionWrapper::wrScissor(__VA_ARGS__)
#define glViewport(...) opengl::FunctionWrapper::wrViewport(__VA_ARGS__)
#define glBindTexture(...) opengl::FunctionWrapper::wrBindTexture(__VA_ARGS__)
#define glTexImage2D(...) opengl::FunctionWrapper::wrTexImage2D(__VA_ARGS__)
#define glTexParameteri(...) opengl::FunctionWrapper::wrTexParameteri(__VA_ARGS__)
#define glGetIntegerv(...) opengl::FunctionWrapper::wrGetIntegerv(__VA_ARGS__)
#define glGetString(...) opengl::FunctionWrapper::wrGetString(__VA_ARGS__)
#define glReadPixels(...) opengl::FunctionWrapper::wrReadPixels(__VA_ARGS__)
#define glTexSubImage2D(...) opengl::FunctionWrapper::wrTexSubImage2D(__VA_ARGS__)
#define glDrawArrays(...) opengl::FunctionWrapper::wrDrawArrays(__VA_ARGS__)
#define glDrawElements(...) opengl::FunctionWrapper::wrDrawElements(__VA_ARGS__)
#define glLineWidth(...) opengl::FunctionWrapper::wrLineWidth(__VA_ARGS__)
#define glClear(...) opengl::FunctionWrapper::wrClear(__VA_ARGS__)
#define glGetFloatv(...) opengl::FunctionWrapper::wrGetFloatv(__VA_ARGS__)
#define glDeleteTextures(...) opengl::FunctionWrapper::wrDeleteTextures(__VA_ARGS__)
#define glGenTextures(...) opengl::FunctionWrapper::wrGenTextures(__VA_ARGS__)
#define glTexParameterf(...) opengl::FunctionWrapper::wrTexParameterf(__VA_ARGS__)
#define glActiveTexture(...) opengl::FunctionWrapper::wrActiveTexture(__VA_ARGS__)
#define glBlendColor(...) opengl::FunctionWrapper::wrBlendColor(__VA_ARGS__)
#define glReadBuffer(...) opengl::FunctionWrapper::wrReadBuffer(__VA_ARGS__)
#define glFinish(...) opengl::FunctionWrapper::wrFinish(__VA_ARGS__)
#if defined(OS_ANDROID)
#define eglGetNativeClientBufferANDROID(...) opengl::FunctionWrapper::ewrGetNativeClientBufferANDROID(__VA_ARGS__)
#endif
#define glCreateShader(...) opengl::FunctionWrapper::wrCreateShader(__VA_ARGS__)
#define glCompileShader(...) opengl::FunctionWrapper::wrCompileShader(__VA_ARGS__)
#define glShaderSource(...) opengl::FunctionWrapper::wrShaderSource(__VA_ARGS__)
#define glCreateProgram(...) opengl::FunctionWrapper::wrCreateProgram(__VA_ARGS__)
#define glAttachShader(...) opengl::FunctionWrapper::wrAttachShader(__VA_ARGS__)
#define glLinkProgram(...) opengl::FunctionWrapper::wrLinkProgram(__VA_ARGS__)
#define glUseProgram(...) opengl::FunctionWrapper::wrUseProgram(__VA_ARGS__)
#define glGetUniformLocation(...) opengl::FunctionWrapper::wrGetUniformLocation(__VA_ARGS__)
#define glUniform1i(...) opengl::FunctionWrapper::wrUniform1i(__VA_ARGS__)
#define glUniform1f(...) opengl::FunctionWrapper::wrUniform1f(__VA_ARGS__)
#define glUniform2f(...) opengl::FunctionWrapper::wrUniform2f(__VA_ARGS__)
#define glUniform2i(...) opengl::FunctionWrapper::wrUniform2i(__VA_ARGS__)
#define glUniform4i(...) opengl::FunctionWrapper::wrUniform4i(__VA_ARGS__)

#define glUniform4f(...) opengl::FunctionWrapper::wrUniform4f(__VA_ARGS__)
#define glUniform3fv(...) opengl::FunctionWrapper::wrUniform3fv(__VA_ARGS__)
#define glUniform4fv(...) opengl::FunctionWrapper::wrUniform4fv(__VA_ARGS__)
#define glDetachShader(...) opengl::FunctionWrapper::wrDetachShader(__VA_ARGS__)
#define glDeleteShader(...) opengl::FunctionWrapper::wrDeleteShader(__VA_ARGS__)
#define glDeleteProgram(...) opengl::FunctionWrapper::wrDeleteProgram(__VA_ARGS__)
#define glGetProgramInfoLog(...) opengl::FunctionWrapper::wrGetProgramInfoLog(__VA_ARGS__)
#define glGetShaderInfoLog(...) opengl::FunctionWrapper::wrGetShaderInfoLog(__VA_ARGS__)
#define glGetShaderiv(...) opengl::FunctionWrapper::wrGetShaderiv(__VA_ARGS__)
#define glGetProgramiv(...) opengl::FunctionWrapper::wrGetProgramiv(__VA_ARGS__)

#define glEnableVertexAttribArray(...) opengl::FunctionWrapper::wrEnableVertexAttribArray(__VA_ARGS__)
#define glDisableVertexAttribArray(...) opengl::FunctionWrapper::wrDisableVertexAttribArray(__VA_ARGS__)
#define glVertexAttribPointer(...) opengl::FunctionWrapper::wrVertexAttribPointer(__VA_ARGS__)
#define glBindAttribLocation(...) opengl::FunctionWrapper::wrBindAttribLocation(__VA_ARGS__)
#define glVertexAttrib1f(...) opengl::FunctionWrapper::wrVertexAttrib1f(__VA_ARGS__)
#define glVertexAttrib4f(...) opengl::FunctionWrapper::wrVertexAttrib4f(__VA_ARGS__)
#define glVertexAttrib4fv(...) opengl::FunctionWrapper::wrVertexAttrib4fv(__VA_ARGS__)

#define glDepthRangef(...) opengl::FunctionWrapper::wrDepthRangef(__VA_ARGS__)
#define glClearDepthf(...) opengl::FunctionWrapper::wrClearDepthf(__VA_ARGS__)

#define glBindBuffer(...) opengl::FunctionWrapper::wrBindBuffer(__VA_ARGS__)
#define glBindFramebuffer(...) opengl::FunctionWrapper::wrBindFramebuffer(__VA_ARGS__)
#define glBindRenderbuffer(...) opengl::FunctionWrapper::wrBindRenderbuffer(__VA_ARGS__)
#define glDrawBuffers(...) opengl::FunctionWrapper::wrDrawBuffers(__VA_ARGS__)
#define glGenFramebuffers(...) opengl::FunctionWrapper::wrGenFramebuffers(__VA_ARGS__)
#define glDeleteFramebuffers(...) opengl::FunctionWrapper::wrDeleteFramebuffers(__VA_ARGS__)
#define glFramebufferTexture2D(...) opengl::FunctionWrapper::wrFramebufferTexture2D(__VA_ARGS__)
#define glTexImage2DMultisample(...) opengl::FunctionWrapper::wrTexImage2DMultisample(__VA_ARGS__)
#define glTexStorage2DMultisample(...) opengl::FunctionWrapper::wrTexStorage2DMultisample(__VA_ARGS__)
#define glGenRenderbuffers(...) opengl::FunctionWrapper::wrGenRenderbuffers(__VA_ARGS__)
#define glRenderbufferStorage(...) opengl::FunctionWrapper::wrRenderbufferStorage(__VA_ARGS__)
#define glDeleteRenderbuffers(...) opengl::FunctionWrapper::wrDeleteRenderbuffers(__VA_ARGS__)
#define glFramebufferRenderbuffer(...) opengl::FunctionWrapper::wrFramebufferRenderbuffer(__VA_ARGS__)
#define glCheckFramebufferStatus(...) opengl::FunctionWrapper::wrCheckFramebufferStatus(__VA_ARGS__)
#define glBlitFramebuffer(...) opengl::FunctionWrapper::wrBlitFramebuffer(__VA_ARGS__)
#define glGenVertexArrays(...) opengl::FunctionWrapper::wrGenVertexArrays(__VA_ARGS__)
#define glBindVertexArray(...) opengl::FunctionWrapper::wrBindVertexArray(__VA_ARGS__)
#define glDeleteVertexArrays(...) opengl::FunctionWrapper::wrDeleteVertexArrays(__VA_ARGS__);
#define glGenBuffers(...) opengl::FunctionWrapper::wrGenBuffers(__VA_ARGS__)
#define glBufferData(...) opengl::FunctionWrapper::wrBufferData(__VA_ARGS__)
#define glMapBuffer(...) opengl::FunctionWrapper::wrMapBuffer(__VA_ARGS__)
#define glMapBufferRange(...) opengl::FunctionWrapper::wrMapBufferRange(__VA_ARGS__)
#define glUnmapBuffer(...) opengl::FunctionWrapper::wrUnmapBuffer(__VA_ARGS__)
#define glDeleteBuffers(...) opengl::FunctionWrapper::wrDeleteBuffers(__VA_ARGS__)
#define glBindImageTexture(...) opengl::FunctionWrapper::wrBindImageTexture(__VA_ARGS__)
#define glMemoryBarrier(...) opengl::FunctionWrapper::wrMemoryBarrier(__VA_ARGS__)
#define glGetStringi(...) opengl::FunctionWrapper::wrGetStringi(__VA_ARGS__)
#define glInvalidateFramebuffer(...) opengl::FunctionWrapper::wrInvalidateFramebuffer(__VA_ARGS__)
#define glBufferStorage(...) opengl::FunctionWrapper::wrBufferStorage(__VA_ARGS__)
#define glFenceSync(...) opengl::FunctionWrapper::wrFenceSync(__VA_ARGS__)
#define glClientWaitSync(...) opengl::FunctionWrapper::wrClientWaitSync(__VA_ARGS__)
#define glDeleteSync(...) opengl::FunctionWrapper::wrDeleteSync(__VA_ARGS__)

#define glGetUniformBlockIndex(...) opengl::FunctionWrapper::wrGetUniformBlockIndex(__VA_ARGS__)
#define glUniformBlockBinding(...) opengl::FunctionWrapper::wrUniformBlockBinding(__VA_ARGS__)
#define glGetActiveUniformBlockiv(...) opengl::FunctionWrapper::wrGetActiveUniformBlockiv(__VA_ARGS__)
#define glGetUniformIndices(...) opengl::FunctionWrapper::wrGetUniformIndices(__VA_ARGS__)
#define glGetActiveUniformsiv(...) opengl::FunctionWrapper::wrGetActiveUniformsiv(__VA_ARGS__)
#define glBindBufferBase(...) opengl::FunctionWrapper::wrBindBufferBase(__VA_ARGS__)
#define glBufferSubData(...) opengl::FunctionWrapper::wrBufferSubData(__VA_ARGS__)

#define glGetProgramBinary(...) opengl::FunctionWrapper::wrGetProgramBinary(__VA_ARGS__)
#define glProgramBinary(...) opengl::FunctionWrapper::wrProgramBinary(__VA_ARGS__)
#define glProgramParameteri(...) opengl::FunctionWrapper::wrProgramParameteri(__VA_ARGS__)

#define glTexStorage2D(...) opengl::FunctionWrapper::wrTexStorage2D(__VA_ARGS__)
#define glTextureStorage2D(...) opengl::FunctionWrapper::wrTextureStorage2D(__VA_ARGS__)
#define glTextureSubImage2D(...) opengl::FunctionWrapper::wrTextureSubImage2D(__VA_ARGS__)
#define glTextureStorage2DMultisample(...) opengl::FunctionWrapper::wrTextureStorage2DMultisample(__VA_ARGS__)
#define glTextureParameteri(...) opengl::FunctionWrapper::wrTextureParameteri(__VA_ARGS__)
#define glTextureParameterf(...) opengl::FunctionWrapper::wrTextureParameterf(__VA_ARGS__)
#define glCreateTextures(...) opengl::FunctionWrapper::wrCreateTextures(__VA_ARGS__)
#define glCreateBuffers(...) opengl::FunctionWrapper::wrCreateBuffers(__VA_ARGS__)
#define glCreateFramebuffers(...) opengl::FunctionWrapper::wrCreateFramebuffers(__VA_ARGS__)
#define glNamedFramebufferTexture(...) opengl::FunctionWrapper::wrNamedFramebufferTexture(__VA_ARGS__)
#define glDrawRangeElementsBaseVertex(...) opengl::FunctionWrapper::wrDrawRangeElementsBaseVertex(__VA_ARGS__)
#define glFlushMappedBufferRange(...) opengl::FunctionWrapper::wrFlushMappedBufferRange(__VA_ARGS__)
#define glTextureBarrier(...) opengl::FunctionWrapper::wrTextureBarrier(__VA_ARGS__)
#define glTextureBarrierNV(...) opengl::FunctionWrapper::wrTextureBarrierNV(__VA_ARGS__)
#define glClearBufferfv(...) opengl::FunctionWrapper::wrClearBufferfv(__VA_ARGS__)
#define glEnablei(...) opengl::FunctionWrapper::wrEnablei(__VA_ARGS__)
#define glDisablei(...) opengl::FunctionWrapper::wrDisablei(__VA_ARGS__)
#define glCopyTexImage2D(...) opengl::FunctionWrapper::wrCopyTexImage2D(__VA_ARGS__)
#define glDebugMessageCallback(...) opengl::FunctionWrapper::wrDebugMessageCallback(__VA_ARGS__)
#define glDebugMessageControl(...) opengl::FunctionWrapper::wrDebugMessageControl(__VA_ARGS__)
#define glEGLImageTargetTexture2DOES(...) opengl::FunctionWrapper::wrEGLImageTargetTexture2DOES(__VA_ARGS__)
#define glEGLImageTargetRenderbufferStorageOES(...) opengl::FunctionWrapper::wrEGLImageTargetRenderbufferStorageOES(__VA_ARGS__)
#endif

#define GL_TEXTURE_EXTERNAL_OES 0x8D65

#include "Graphics/OpenGLContext/ThreadedOpenGl/opengl_Wrapper.h"

#endif // GLFUNCTIONS_H
