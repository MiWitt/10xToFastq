/* 
 * File:   main.cpp
 * Author: mwittig
 *
 * Created on July 20, 2017, 9:08 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <libgen.h>
#include <zlib.h>
#include <gzstream.h>
#include <mytools.h>
#include <string.h>


#include <tclap/CmdLine.h>




using namespace std;

#define READONEFILE "--R1"
#define READTWOFILE "--R2"
/*
 * 
 */
int main(int argc, char** argv) 
{
    TCLAP::CmdLine cmd("10x fastq converter:\nR1 and R2 are the two fastq files of a paired end run assuming that both files are synchronized, which means they have their paired reads at the same line numbers. Output goes to the same directory where two files are created with the same filename, except that .fastq.gz is replaced by .recoded.fastq.gz. This software cuts off the index of read 1 and changes the paired end numbering of the fastq header accordingly.", ' ', "0.9");
    try
    {
        TCLAP::SwitchArg reverseSwitch("v","verbose","log status to std::err", cmd, false);
        TCLAP::ValueArg<unsigned int> indexLength("i","index","length of the 10x index",false,16,"unsigned int");
        cmd.add( indexLength );
        TCLAP::ValueArg<std::string> fastqR2Arg("2","R2","R2 fastq file",true,"sample_L004_R2_001.fastq.gz","string");
        cmd.add( fastqR2Arg );
        TCLAP::ValueArg<std::string> fastqR1Arg("1","R1","R1 fastq file",true,"sample_L004_R1_001.fastq.gz","string");
        cmd.add( fastqR1Arg );
        
        cmd.parse(argc,argv);
    
        string fileR1 = fastqR1Arg.getValue();
        string fileR2 = fastqR2Arg.getValue();
        unsigned int idxLength = indexLength.getValue();

        if(fileR1.substr(fileR1.length()-9,9).compare(".fastq.gz") != 0 || fileR2.substr(fileR2.length()-9,9).compare(".fastq.gz") != 0)
            throw CMyException("Only *.fastq.gz files are allowed for input");
        
        igzstream inR1(fileR1.c_str());
        igzstream inR2(fileR2.c_str());
        if(!inR1)
            throw CMyException(string("failed to open ")+fileR1);
        if(!inR2)
            throw CMyException(string("failed to open ")+fileR1);

        string outfileR1 = fileR1.substr(0,fileR1.length()-9).append(".recoded.fastq.gz");
        string outfileR2 = fileR2.substr(0,fileR2.length()-9).append(".recoded.fastq.gz");
        ogzstream  outR1(outfileR1.c_str());
        ogzstream  outR2(outfileR2.c_str());
        if(!outR1)
            throw CMyException(string("failed to open ")+outfileR1);
        if(!outR2)
            throw CMyException(string("failed to open ")+outfileR2);

        map<string, size_t> index;
        map<string, pair<string,string> > header;

        size_t failed = 0;
        size_t worked = 0;
        size_t seq_eeror_in_idx = 0;
        string strR1_lines[4];
        string strR2_lines[4];
        while(    getline(inR1,strR1_lines[0]) && getline(inR1,strR1_lines[1]) && getline(inR1,strR1_lines[2]) && getline(inR1,strR1_lines[3])
               && getline(inR2,strR2_lines[0]) && getline(inR2,strR2_lines[1]) && getline(inR2,strR2_lines[2]) && getline(inR2,strR2_lines[3]) )
        {

            if(worked%100000 == 0 && worked != 0 && reverseSwitch.getValue())
                cerr << " .... " << worked << " valid paired ends with " << index.size() << " different indices" << endl;
            
            string idx = strR1_lines[1].substr(0,idxLength);
            if(idx.length() == idxLength)
            {
                if(idx.find_first_not_of("ACGT") != string::npos)
                {
                    seq_eeror_in_idx++;
                    continue;
                }
                vector<string> split = CMyTools::GetParsedLine(strR1_lines[0]," ");
                map<string, size_t>::iterator iterIdx = index.find(idx);
                map<string, pair<string,string> >::iterator iterHeader = header.end();
                if(iterIdx == index.end())
                {
                    iterIdx = index.insert(pair<string,size_t>(idx,1)).first;
                    iterHeader = header.insert(pair<string, pair<string,string> >(idx,pair<string,string>(split[0],split[1]))).first;
                }
                else
                {
                    iterIdx->second++;
                    iterHeader = header.find(idx);
                }
                size_t act_count = iterIdx->second*2-1;

                string strHeaderPart2Core = iterHeader->second.second.substr(iterHeader->second.second.find(':'));

                outR1 << iterHeader->second.first << ' ' << act_count++ << strHeaderPart2Core << endl;
                outR1 << strR1_lines[1].substr(idxLength) << endl;
                outR1 << strR1_lines[2] << endl;
                outR1 << strR1_lines[3] << endl;
                outR2 << iterHeader->second.first << ' ' << act_count << strHeaderPart2Core << endl;
                outR2 << strR2_lines[1] << endl;
                outR2 << strR2_lines[2] << endl;
                outR2 << strR2_lines[3] << endl;

                worked++;
            }
            else
            {
                failed++;
            }
        }

        inR1.close();
        inR2.close();
        outR1.close();
        outR2.close();

        cerr << "stats:" << endl
             << "paired reads worked: " << worked << endl
             << "paired reads failed: " << failed << endl
             << "sequencing error within index: " << seq_eeror_in_idx << endl;
        return 0;
    }
    catch(TCLAP::ArgException &e)
    {
        cerr << "error: " << e.error() << " for arg " << e.argId() << " in "  << argv[0] << endl;
    }
    catch(exception &e)
    {
        cerr << "error: " << e.what() << " in " << argv[0] << endl;
    }
    catch(CMyException &e)
    {
        cerr << "error: " << e.what() << " in " << argv[0] << endl;
    }
    catch(...)
    {
        cerr << "unexpected error in " << argv[0] << endl;
    }
    return (EXIT_FAILURE);
}

