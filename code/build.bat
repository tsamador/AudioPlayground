@echo off
mkdir ..\build
pushd ..\build
cl -Zi /w /EHsc /MTd ../code/gl_synes.cpp ../code/glad.c ../code/synesthetic.cpp ../code/audio_engine.cpp ../code/sound_buffer.cpp /link  /LIBPATH:..\lib OpenGL32.lib glfw3.lib user32.lib gdi32.lib Shell32.lib Ole32.lib Winmm.lib
popd

