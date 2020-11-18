# Hierarchial Meta-Storms

![Version](https://img.shields.io/badge/Version-1.01%20-brightgreen)
![Release date](https://img.shields.io/badge/Released%20date-Nov.%2018%2C%202020-brightgreen)



# Contents

- [Introduction](#introduction)

- [System Requirement and dependency](#system-requirement-and-dependency)

- [Installation guide](#installation-guide)

- [Usage](#usage)

- [Example dataset](#example-dataset)

- [Tools in this package](#tools-in-this-package)

- [Supplementary](#supplementary)

  

# Introduction

Hierarchical Meta-Storms (HMS) comprehensively calculates the dissimilarities of microbiome functional profiles by considering multi-level metabolic pathway hierarchy. It contains two core components: *i)* a dissimilarity algorithm that comprehensively calculates the distances among microbiome functional profiles by considering their multi-level metabolic pathway hierarchy among functional gene families, and *ii)* a PCoA implementation optimized by multi-thread parallel computing to rapidly parse out the beta-diversity pattern for thousands of samples. It takes the microbiome functional profiles of KO and their relative abundance as input, and computes and outputs their pairwise distance matrix and then the principle coordinates of PCoA. 

In addition, we also encapsulate the C++ source codes as an R package （![#Github: hrms](https://github.com/qdu-bioinfo/hrms.git)) using RcppArmadillo framework and submitted to CRAN, making the kernel functions of calculating the microbiome distances and principle coordinates could be callable by R interpreter in both R terminal and R scripts.

# System Requirement and dependency

## Hardware Requirements

Hierarchical Meta-Storms only requires a standard computer with sufficient RAM to support the operations defined by a user. For typical users, this would be a computer with about 2 GB of RAM. For optimal performance, we recommend a computer with the following specs:

  RAM: 8+ GB  
  CPU: 4+ cores, 3.3+ GHz/core

## Software Requirements

OpenMP library is the C/C++ parallel computing library. Most Linux releases have OpenMP already been installed in the system. In Mac OS X, to install the compiler that supports OpenMP, we recommend using the Homebrew package manager:
```
brew install gcc
```

# Installation guide

## Automatic Installation (recommended)

At present, Hierarchical Meta-Storms provides a fully automatic installer for easy installation.

**a. Download the package**

```
git clone https://github.com/qdu-bioinfo/hierarchical-meta-storms.git	
```

**b. Install by installer**
```
cd hierarchical-meta-storms
source install.sh
```

The package should take less than 1 minute to install on a computer with the specifications recommended above.

The example dataset could be found at “example” folder. Check the “example/Readme” for details about the demo run.

## Manual Installation

If the automatic installer fails, Dynamic Meta-Storms can still be installed manually.

**a. Download the package**
```
git clone https://github.com/qibebt-bioinfo/hierarchical-meta-storms.git	
```

**b. Configure the environment variables (the default environment variable configuration file is “~/.bashrc”)**
```
export HierarchicalMetaStorms=Path to Hierarchical Meta-Storms
export PATH=”$PATH:$HierarchicalMetaStorms/bin/”
source ~/.bashrc
```
**c. Compile the source code**

```
cd hierarchical-meta-storms
make
```
# Usage
**a.  Compute the distance matrix**

```
MS-comp-func -T samples.ko.table -o samples.func.dist
```
The output file “samples.func.dist” is the pairwise distance matrix. 

**b. Implement the PCoA**

```
MS-get-pc -d samples.func.dist -o samples.pcoa.pc
```
The output file “samples.pcoa.pc” is the coordinates results of PCoA. 

# Example dataset
Here we provide a demo dataset (Synthetic Dataset 1) with functional KO profiles of 30 artificial microbiomes in “example” folder. In this package, “dataset1.ko.abd” is the relative abundance on KOs-level, and “dataset1.meta” is the group information of the samples.

To run the demo, you can either:
```
cd example
sh Readme
```
or type the following command:
```
MS-comp-func -T dataset1.ko.abd -o dataset1.dist
MS-get-pc -d dataset1.dist -m dataset1.meta -o dataset1.pcoa.pc
```
Then the output file “dataset1.dist” is the pairwise distance of the 30 samples and "dataset1.pcoa.pc" is the coordinates results of PCoA based on the distance matrix. 

This demo run should take less than 1 minute on a recommended computer.

# Tools in this package
**a. MS-comp-func**

It calculates the hierarchical meta-storms distance matrix among microbiome functional profiles. Run:
```
MS-comp-func -h
```
for detailed parameters.

**b. MS-get-pc**

It calculates the PCoA based the distance matrix. Run:
```
MS-get-pc -h
```
for detailed parameters.


# Supplementary

[Synthetic Dataset 1](http://bioinfo.single-cell.cn/Released_Software/hierarchical-meta-storms/data/synthetic_dataset_1.tar.gz) contains 30 synthetic samples.

[Real Dataset 1](http://bioinfo.single-cell.cn/Released_Software/hierarchical-meta-storms/data/real_dataset_1.tar.gz) contains 20,000 real samples that are inferred from 16S rRNA gene by PICRUSt 2.

[Real Dataset 2](http://bioinfo.single-cell.cn/Released_Software/hierarchical-meta-storms/data/actual_dataset_2.tar.gz) contains 5,350 real samples that are inferred from 16S rRNA gene by PICRUSt 2.

[Real Dataset 3](http://bioinfo.single-cell.cn/Released_Software/hierarchical-meta-storms/data/actual_dataset_3.tar.gz) contains 2,354 real samples, reconstructed from WGS data by HUMANn 2.




