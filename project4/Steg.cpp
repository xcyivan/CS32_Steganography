#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) {
    if(hostIn=="")  return false;

    //--------seperate hostIn--------------------
    vector<string> seperatedHost;
    string fragment;
    for(int i=0; i<hostIn.size(); i++){
        
        while(hostIn[i]!='\n' && i<hostIn.size()){
            fragment+=hostIn[i];
            i++;
        }
        if(fragment.size()>0 && fragment[fragment.size()-1]=='\r')
            fragment.erase(fragment.size()-1,fragment.size());
        seperatedHost.push_back(string(fragment));
        fragment="";
    }
    if(fragment!="")
        seperatedHost.push_back(string(fragment));
    
    //--------cut tailing space and tab----------
    vector<string> cutTail;
    for(int i=0; i<seperatedHost.size(); i++){
        string st=seperatedHost[i];
        while(st.size()>0 && (st[st.size()-1]=='\t' || st[st.size()-1]==' '))
            st.erase(st.size()-1,st.size());
        cutTail.push_back(string(st));
    }
    //--------convert msg to space and tab-------
    vector<unsigned short> V;
    Compressor::compress(msg, V);
    string hiddenMsg = BinaryConverter::encode(V);

    //--------seperate hidden msg----------------
    int N=cutTail.size();
    int L=hiddenMsg.size();
    vector<string> seperatedHiddenMsg;
    int k=1;
    for(int i=0; i<hiddenMsg.size();){
        string fragment;
        if(k<=L%N){
            fragment=hiddenMsg.substr(i,(L/N)+1);
            i+=(L/N+1);
        }
        else{
            fragment=hiddenMsg.substr(i,L/N);
            i+=(L/N);
        }
        k++;
        seperatedHiddenMsg.push_back(string(fragment));
    }
    //----combine seperated Host and hidden msg----
    for(int i=0; i<cutTail.size(); i++){
        hostOut+=(cutTail[i]+seperatedHiddenMsg[i]+"\n");
    }
	return true;
}




bool Steg::reveal(const string& hostIn, string& msg) {
    //--------seperate hostIn--------------------
    vector<string> seperatedHost;
    string fragment;
    for(int i=0; i<hostIn.size(); i++){
        
        while(hostIn[i]!='\n' && i<hostIn.size()){
            fragment+=hostIn[i];
            i++;
        }
        if(fragment.size()>0 && fragment[fragment.size()-1]=='\r')
            fragment.erase(fragment.size()-1,fragment.size());
        seperatedHost.push_back(string(fragment));
        fragment="";
    }
    if(fragment!="")
        seperatedHost.push_back(string(fragment));
    //---------extract space and tab-------------
    string hiddenMsg;
    for(int i=0; i<seperatedHost.size(); i++){
        string fragment=seperatedHost[i];
        int j=fragment.size()-1;
        while(j>=0&&(fragment[j]=='\t' || fragment[j]==' '))
            j--;
        hiddenMsg+=fragment.substr(j+1,fragment.size()-j-1);
    }
    //---------decode hidden msg-----------------
    vector<unsigned short> V;
    if(!BinaryConverter::decode(hiddenMsg, V))  return false;
    if(!Compressor::decompress(V, msg)) return false;
	return true;
}
