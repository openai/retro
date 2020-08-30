.. _development:

Development
=====================================

Adding new games can be done without recompiling Gym Retro, but if you need to work on the C++ code or make changes to the UI, you will want to compile Gym Retro from source.

Install Retro from source
--------------------------------------

Building Gym Retro requires at least either gcc 5 or clang 3.4.

Prerequisites
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To build Gym Retro you must first install CMake.
You can do this either through your package manager, download from the `official site <https://cmake.org/download/>`_ or ``pip3 install cmake``.
If you're using the official installer on Windows, make sure to tell CMake to add itself to the system PATH.

Mac prerequisites
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Since LuaJIT does not work properly on macOS you must first install Lua 5.1 from homebrew:

.. code-block:: shell

    brew install pkg-config lua@5.1

Windows prerequisites
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you are not on Windows, please skip to the next section.
Otherwise, you will also need to download and install `Git <https://git-scm.com/downloads>`_ and `MSYS2 x86_64 <http://www.msys2.org>`_.
When you install git, choose to use Git from the Windows Command Prompt.

After you have installed msys2 open an MSYS2 MinGW 64-bit prompt (under Start > MSYS2 64bit)  and run this command:

.. code-block:: shell

    pacman -Sy make mingw-w64-x86_64-gcc


Once that's done, close the prompt and open a Git CMD prompt (under Start > Git) and run these commands.
If you installed MSYS2 into an alternate directory please use that instead of C:\msys64 in the command.

.. code-block:: bat

    path %PATH%;C:\msys64\mingw64\bin;C:\msys64\usr\bin
    set MSYSTEM=MINGW64

Then in the same prompt, without closing it first, continue with the steps in the next section.
If you close the prompt you will need to rerun the last commands before you can rebuild.

Linux prerequisites
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: shell

    sudo apt-get install zlib1g-dev
    
Building
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: shell

    git clone https://github.com/openai/retro.git gym-retro
    cd gym-retro
    pip3 install -e .

Install Retro UI from source
--------------------------------------

First make sure you can install Retro from source, after that follow the instructions for your platform:

macOS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note that for Mojave (10.14) you may need to install ``/Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg``

.. code-block:: shell

    brew install pkg-config capnp lua@5.1 qt5
    cmake . -DCMAKE_PREFIX_PATH=/usr/local/opt/qt -DBUILD_UI=ON -UPYLIB_DIRECTORY
    make -j$(sysctl hw.ncpu | cut -d: -f2)
    open "Gym Retro Integration.app"


Linux
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


.. code-block:: shell

    sudo apt-get install capnproto libcapnp-dev libqt5opengl5-dev qtbase5-dev zlib1g-dev
    cmake . -DBUILD_UI=ON -UPYLIB_DIRECTORY
    make -j$(grep -c ^processor /proc/cpuinfo)
    ./gym-retro-integration

Windows
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Building from source on Windows is currently difficult to configure. Docker containers for cross-compiling are available at `openai/travis-build <https://hub.docker.com/r/openai/travis-build/>`_.
