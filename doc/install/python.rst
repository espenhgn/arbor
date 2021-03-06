.. _in_python:

Python Installation
===================

Arbor's Python API will be the most convenient interface for most users. Note that we only support Python 3.6 and later.
Any instruction hereafter assumes you're using `python` and `pip` no older than that.

Getting Arbor
-------------

The easiest way to get Arbor is with
`pip <https://packaging.python.org/tutorials/installing-packages>`_:

.. code-block:: bash

    pip3 install arbor --user

Every point release is pushed to the Python Package Index. If you wish to install another version, it is possible to use
Setuptools directly on a local copy of the source code, or instruct `pip` to install directly from our git repository:

.. code-block:: bash

    # use setuptools and python directly
    git clone https://github.com/arbor-sim/arbor.git --recursive
    python3 install ./arbor/setup.py

    # tell pip to build and install from master
    pip3 install git+https://github.com/arbor-sim/arbor.git --user

.. note::
    You will need to have some development packages installed in order to build Arbor this way.

    * Ubuntu/Debian: `sudo apt install git build-essential python3-dev python3-pip`
    * Fedora/CentOS/Red Hat: `sudo yum install git @development-tools python3-devel python3-pip`
    * macOS: get `brew` `here <https://brew.sh>`_ and run `brew install cmake clang python3`
    * Windows: the simplest way is to use `WSL <https://docs.microsoft.com/en-us/windows/wsl/install-win10>`_ and then follow the instructions for Ubuntu.

To test that Arbor is available, try the following in a Python interpreter
to see information about the version and enabled features:

.. code-block:: python

    >>> import arbor
    >>> print(arbor.__version__)
    >>> print(arbor.__config__)

You are now ready to use Arbor! You can continue reading these documentation pages, have a look at the
:ref:`Python API reference<pyoverview>` , or visit the :ref:`Quick Start page<gs_single_cell>`.

.. Note::
    To get help in case of problems installing with pip, run pip with the ``--verbose`` flag, and attach the output
    (along with the pip command itself) to a ticket on `Arbor's issues page <https://github.com/arbor-sim/arbor/issues>`_.

Advanced options
^^^^^^^^^^^^^^^^^^

By default Arbor is installed with multi-threading enabled.
To enable more advanced forms of parallelism, the following optional flags can
be used to configure the installation:

* ``--mpi``: Enable MPI support (requires MPI library).
* ``--gpu``: Enable GPU support for NVIDIA GPUs with nvcc using ``cuda``, or with clang using ``cuda-clang`` (both require cudaruntime).
  Enable GPU support for AMD GPUs with hipcc using ``hip``. By default set to ``none``, which disables gpu support.
* ``--vec``: Enable vectorization. This might require choosing an appropriate architecture using ``--arch``.
* ``--arch``: CPU micro-architecture to target. By default this is set to ``native``.

If calling ``setup.py`` the flags must come after ``install`` on the command line,
and if being passed to pip they must be passed via ``--install-option``. The examples
below demonstrate this for both pip and ``setup.py``.

**Vanilla install** with no additional features enabled:

.. code-block:: bash

    pip3 install arbor
    python3 ./arbor/setup.py install

**With MPI support**. This might require loading an MPI module or setting the ``CC`` and ``CXX``
:ref:`environment variables <install-mpi>`:

.. code-block:: bash

    pip3 install --install-option='--mpi' ./arbor
    python3 ./arbor/setup.py install --mpi

**Compile with** :ref:`vectorization <install-vectorize>` on a system with a SkyLake
:ref:`architecture <install-architecture>`:

.. code-block:: bash

    pip3 install --install-option='--vec' --install-option='--arch=skylake' arbor
    python3 ./arbor/setup.py install --vec --arch=skylake

**Enable NVIDIA GPUs (compiled with nvcc)**. This requires the :ref:`CUDA toolkit <install-gpu>`:

.. code-block:: bash

    pip3 install --install-option='--gpu=cuda' ./arbor
    python3 ./arbor/setup.py install  --gpu=cuda

**Enable NVIDIA GPUs (compiled with clang)**. This also requires the :ref:`CUDA toolkit <install-gpu>`:

.. code-block:: bash

    pip3 install --install-option='--gpu=cuda-clang' ./arbor
    python3 ./arbor/setup.py install --gpu=cuda-clang

**Enable AMD GPUs (compiled with hipcc)**. This requires setting the ``CC`` and ``CXX``
:ref:`environment variables <install-gpu>`

.. code-block:: bash

    pip3 install --install-option='--gpu=hip' ./arbor
    python3 ./arbor/setup.py install --gpu=hip

.. Note::
    Setuptools compiles the Arbor C++ library and
    wrapper, which can take a few minutes. Pass the ``--verbose`` flag to pip
    to see the individual steps being performed if you are concerned that progress
    is halting.

.. Note::
    Detailed instructions on how to install using CMake are in the
    :ref:`Python configuration <install-python>` section of the
    :ref:`installation guide <in_build_install>`.
    CMake is recommended for developers, integration with package managers such as
    Spack and EasyBuild, and users who require fine grained control over compilation
    and installation.

Dependencies
^^^^^^^^^^^^^

If a downstream dependency requires Arbor be built with
a specific feature enabled, use ``requirements.txt`` to
`define the constraints <https://pip.pypa.io/en/stable/reference/pip_install/#per-requirement-overrides>`_.
For example, a package that depends on `arbor` version 0.3 or later
with MPI support would add the following to its requirements:

.. code-block:: python

    arbor >= 0.3 --install-option='--gpu=cuda' \
                 --install-option='--mpi'

Performance
--------------

The Python interface can incur significant memory and runtime overheads relative to C++
during the *model building* phase, however simulation performance is the same
for both interfaces.
