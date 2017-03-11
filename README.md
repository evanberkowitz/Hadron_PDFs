# Hadron_PDFs
Computing Hadronic PDFs with the FH technique.

We aim to compute hadronic parton distribution functions using the Feynman-Hellman technique.

#Table of Contents

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

#Introduction

Parton Distribution Functions, or PDFs, are functions that characterize the inner structure of hadrons, bound states quarks (and antiquarks) in QCD.

#Method

The [Feynman-Hellman (FH) method][fh] is a technique for measuring hadronic matrix elements, normally requiring a three-point function, into a two-point function which can be analyzed with familiar effective-mass techniques.  Here we apply this technique to PDFs.

The FH method works by computing *continued propagators*---applying an operator to an already-computed propagator, and then using the result as a source for another propagator.

Here, the operator we apply is: injecting a given momentum, transporting a quark along a (spatial) path via the gauge links, and then emitting a (possibly-different) momentum.

# Code


##Dependencies

`Hadron_PDFs` is built on top of [the USQCD software stack][USQCD].  In particular, it requires [`QMP`][QMP], [`QDP++`][QDPXX] with [`HDF5`][HDF5], and [`Chroma`][CHROMA].

##Installation

##Running

#Acknowledgements

This software was developed by Evan Berkowitz, Xiaonu Xiong, and Tom Luu at Forschungszentrum Jülich.

#License

TBD.


[fh]:       https://arxiv.org/abs/1612.06963
[USQCD]:    https://usqcd-software.github.io/
[QMP]:      https://github.com/usqcd-software/qmp
[QDPXX]:    http://usqcd-software.github.io/qdpxx/
[CHROMA]:   https://github.com/JeffersonLab/chroma