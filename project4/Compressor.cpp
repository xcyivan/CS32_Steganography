#include "provided.h"
#include "HashTable.h"
//#include "substituteHashTable.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{

    int N=s.size();
    //int C=min(N/2+512,16384);
    int C = (N / 2 + 512) < 16384 ? (N / 2) + 512 : 16384;
    //int C=257;
    HashTable<string,unsigned short> ht(2*C,C);
    for(int j=0; j<=255; j++){
        char ch=static_cast<char>(j);
        string st(1,ch);
        ht.set(st,j,true);
    }
    int nextFreeID = 256;
    string runSoFar;
    
    for(int i=0; i<s.size(); i++){
        string expandedRun=runSoFar+s[i];
        unsigned short mappedX;
        if(ht.get(expandedRun,mappedX)){
            runSoFar=expandedRun;
            continue;
        }
        else{
            ht.get(runSoFar,mappedX);
            numbers.push_back(mappedX);
            ht.touch(runSoFar);
            runSoFar="";
            string st(1,s[i]);
            unsigned short cv;
            ht.get(st,cv);
            numbers.push_back(cv);
            if(ht.set(expandedRun,nextFreeID))  nextFreeID++;
            else{
                string discardStr;
                unsigned short discardID;
                if(ht.discard(discardStr,discardID))
                    ht.set(expandedRun,discardID);
            }
        }
    }
    if(runSoFar!=""){
        unsigned short mappedX;
        ht.get(runSoFar,mappedX);
        numbers.push_back(mappedX);
    }
    numbers.push_back(C);
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s){   
    unsigned int C=numbers[numbers.size()-1];
    HashTable<unsigned short, string> ht(2*C,C);
    for(int j=0; j<=255; j++){
        char ch=static_cast<char>(j);
        string st(1,ch);
        ht.set(j,st,true);
    }
    //ht.dump();
    int nextFreeID=256;
    string runSoFar;
    string output;
    for(int i=0; i<numbers.size()-1; i++){
        unsigned short us=numbers[i];
        //a.
        if(us<=255){
            //i.
            string singleStr;
            ht.get(us,singleStr);
            output+=singleStr;
            //ii.
            if(runSoFar==""){
                runSoFar=singleStr;
                continue;
            }
            //iii.
            string expandedRun=runSoFar+singleStr;
            //iv.
            if(!ht.isFull()){
                ht.set(nextFreeID,expandedRun);
                nextFreeID++;
            }
            //v.
            else{
                string discardStr;
                unsigned short discardID;
                if(ht.discard(discardID,discardStr))
                    ht.set(discardID,expandedRun);
            }
            //vi.
            runSoFar="";
            continue;
        }
        //b.
        else{
            string multiStr;
            if(!ht.get(us,multiStr))    {cout<<"false"<<endl;return false;}
            else    ht.touch(us);
            output+=multiStr;
            runSoFar=multiStr;
            continue;
        }
    }
    s=output;
	return true;
}
