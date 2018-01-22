# Hadron_PDFs
Computing Hadronic PDFs with the FH technique.

We aim to compute hadronic parton distribution functions using the Feynman-Hellman technique.

# Table of Contents

- [Hadron_PDFs](#hadron_pdfs)
- [Table of Contents](#table-of-contents)
- [Introduction](#introduction)
- [Method](#method)
- [Code](#code)
  - [Dependencies](#dependences)
  - [Installation](#installation)
  - [Running](#running)
- [Acknowledgements](#acknowledgements)
- [License](#license)

# Introduction

Parton Distribution Functions, or PDFs, are functions that characterize the inner structure of hadrons, bound states quarks (and antiquarks) in QCD.

Below is a schematic of the correlation functions of interest.

![PDF](/hadron_pdfs.gif)

The big black circles are supposed to represent baryons, black line on top an incident electron.  The electron talks to valence quarks via photons.  When hard enough, processes like this factorize, and we need only to worry about the behavior of the quarks.  Here, one quark gets hit by a photon, goes a long for a bit, and emits a photon.  Between the two photons we use a Wilson line to transport the quark, rather than letting it propagate according to the Dirac equation.  We want to know the value of the matrix element as a function of the length of this Wilson line, the momenta injected at the electromagnetic interactions, and the asymptotic particle momentum.

# Method

The [Feynman-Hellman (FH) method][fh] is a technique for measuring hadronic matrix elements, normally requiring a three-point function, into a two-point function which can be analyzed with familiar effective-mass techniques.  Here we apply this technique to PDFs.

The FH method works by computing *sequential* or *Feynman-Hellman propagators*---applying an operator to an already-computed propagator, and then using the result as a source for another propagator.

Here, the operator we apply is: injecting a given momentum, transporting a quark along a (spatial) path via the gauge links, and then emitting a (possibly-different) momentum.

# Code


## Dependencies

`Hadron_PDFs` is built on top of [the USQCD software stack][USQCD].  In particular, it requires [`QMP`][QMP], [`QDP++`][QDPXX] with [`HDF5`][HDF5], and [`Chroma`][CHROMA].

## Installation

 - [Install the USQCD software stack][install].
 - Construct a `Makefile`.  The `.gitignore` automatically ignores `Makefile`; instead, write one in the `Makefiles` folder and symbolically link to it.  `Makefiles/mac` provides what is needed, though your compiler and environment description needs to be customized, and the paths to the dependencies are needed in the `DIR_*` variables.
 - `make Hadron_PDFs.x`

## Running

Built on top of chroma, `Hadron_PDFs.x` needs a `-i input_file` argument.  An example input file is provided.

## Comments

When sensible, I've liberally sprinked comments into the code so that it's clear exactly what I'm doing.  This includes drawing ASCII pictures, writing lots of comments for 2 lines of code, and the like.

# Acknowledgements

This software was developed by Evan Berkowitz, Xiaonu Xiong, and Tom Luu at Forschungszentrum Jülich.

# License

TBD.


[fh]:       https://arxiv.org/abs/1612.06963
[USQCD]:    https://usqcd-software.github.io/
[QMP]:      https://github.com/usqcd-software/qmp
[QDPXX]:    http://usqcd-software.github.io/qdpxx/
[CHROMA]:   https://github.com/JeffersonLab/chroma
[HDF5]:     https://www.hdfgroup.org/hdf5/
[install]:  https://github.com/evanberkowitz/usqcd_install