//
//  main.cpp
//  project4
//
//  Created by xiacheyun on 3/6/15.
//  Copyright (c) 2015 xiacheyun. All rights reserved.
//

#include <iostream>
#include <string>
#include "HashTable.h"
#include "provided.h"
#include "http.h"
#include <vector>
using namespace std;


int main(){
    string url="http://cs.ucla.edu";
    string page;
    if(!HTTP().get(url,page))    cout<<"can't load the page"<<endl;
    //page="<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ";
    string testurl="http://xcy.test.com";
    HTTP().set(testurl, page);
    string p;
    string msg="I've hidden some funny information in this webpage but you don't know :P";
    string host_hidden;
    if(!WebSteg::hideMessageInPage(testurl, msg, host_hidden))   cout<<"error hidding msg in page"<<endl;
    string test2url="http://xcy.test2.com";
    HTTP().set(test2url, host_hidden);
    HTTP().get(test2url, p);
    string recMsg;
    if(WebSteg::revealMessageInPage(test2url, recMsg))   cout<<recMsg<<endl;
    
}
//=========================================
//test WebSteg.cpp
//=========================================
//string url="http://cs.ucla.edu";
//string page;
//if(!HTTP().get(url,page))    cout<<"can't load the page"<<endl;
////page="<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ";
//string testurl="http://xcy.test.com";
//HTTP().set(testurl, page);
//string p;
//string msg="I've hidden some funny information in this webpage but you don't know :P";
//string host_hidden;
//if(!WebSteg::hideMessageInPage(testurl, msg, host_hidden))   cout<<"error hidding msg in page"<<endl;
//string test2url="http://xcy.test2.com";
//HTTP().set(test2url, host_hidden);
//HTTP().get(test2url, p);
//string recMsg;
//if(WebSteg::revealMessageInPage(test2url, recMsg))   cout<<recMsg<<endl;

//=========================================
//test Steg.cpp
//=========================================
//string hostIn= "<html>   \nQ \tQQ \t \nBBB\t\t\t   \n\nGG \nBBB \n\t\nDDD\nEEE </html>   ";
//string hostOut;
//string message="ab a";
//Steg::hide(hostIn, message, hostOut);
//cout<<hostOut;
//string recover;
//Steg::reveal(hostOut, recover);


//=========================================
//test Compressor.cpp
//=========================================
//string s="I'll meet you at Bolter 3400 this afternoon!";
//vector<unsigned short> V;
//Compressor::compress(s,V);
//for(int k=0; k<V.size(); k++)
//cout<<V[k]<<" ";
//cout<<endl;
//cout<<"decompress========="<<endl;
//string output;
//Compressor::decompress(V, output);
//cout<<output<<endl;

//=========================================
//test BinaryConverter.cpp
//=========================================
//    string convertNumberToBitString(unsigned short number);
//    vector<unsigned short> v;
//    v.push_back(1);
//    v.push_back(5);
//    v.push_back(3);
//    v.push_back(19);
//    string hiddenMessage = BinaryConverter::encode(v);
//    vector<unsigned short> v2;
//    BinaryConverter::decode(hiddenMessage, v2);
//    for(int i=0; i<v2.size(); i++) cout<<v2[i]<<endl;
//=============end of test=================
    
//=========================================
//test HashTable.h
//=========================================
//    HashTable<unsigned short, int> ht(5,10);
//    ht.set(35,1,true);
//    ht.set(12,2);
//    ht.set(329,3,true);
//    ht.set(332,4);
//    ht.set(167,5);
//    ht.set(3920,6);
//    ht.set(1111,7);
//    ht.set(132,8);
//    ht.set(299,9);
//    ht.set(12,10);
//    ht.set(35,11);
//    ht.touch(35);
//    ht.touch(12);
//    ht.touch(329);
//    ht.touch(332);
//    ht.touch(167);
//    ht.touch(3920);
//    unsigned short k=167;
//    int v=-1;
//    ht.dump();
//    cout<<endl;
//    ht.timely_dump();
//    cout<<"ht.get()="<<ht.get(k,v)<<", v="<<v<<endl;
//    cout<<"discard: "<<ht.discard(k,v)<<" "<<k<<" , "<<v<<endl;
//    cout<<"after discard:======="<<endl;
//    ht.dump();
//    cout<<endl;
//    ht.timely_dump();
//=================end of test===================
