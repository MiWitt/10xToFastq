![](images/ikmb_bfx_logo.png)

# IKMB 10x reads to standard Fastq reads

# Please note

This software reads 10x sequencing files and searches for the molecular identifier in the read. All reads originating from the same droplet will get the same fastq header with different paired end numbering. 

This software uses [gzstream from the filo project ](https://github.com/arq5x/filo) and [TCLAP](https://github.com/eile/tclap) for argument parsing. Both come with their own licences. We assume a system wide zlib is installed.

## Installing the pipeline

To install this pipeline, simply clone the repository to a location:

`git clone https://github.com/MiWitt/10xToFastq.git`

Move to the project directory:

`cd 10xToFastq`

Initialize the submodules:

`git submodule init`

And update the submodules:

`git submodule update`

To compile the tool run make after running a clean (got issues from the sub modules when not cleaning before):

`make clean`
<br />
`make`

To update the code, run git update inside of the local clone:

`git update`

## Usage


USAGE: 

   ./10xToFastq  -1 <string> -2 <string> [-i <unsigned int>] [-v] [--]
                 [--version] [-h]

Where: 

   -1 <string>,  --R1 <string>
     (required)  R1 fastq file

   -2 <string>,  --R2 <string>
     (required)  R2 fastq file

   -i <unsigned int>,  --index <unsigned int>
     length of the 10x index

   -v,  --verbose
     log status to std::err

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.


   10x fastq converter:

   R1 and R2 are the two fastq files of a paired end run assuming that both
   files are synchronized, which means they have their paired reads at the
   same line numbers. Output goes to the same directory where two files are
   created with the same filename, except that .fastq.gz is replaced by
   .recoded.fastq.gz. This software cuts off the index of read 1 and
   changes the paired end numbering of the fastq header accordingly.

## Output

The output will be placed next to the input files. make sure you have write permissions. The output files get the name of the imputfile getting ".recoded." tag left of the fastq.gz suffix.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

