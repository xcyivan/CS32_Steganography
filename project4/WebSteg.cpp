#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
    string page;
    if(!HTTP().get(url, page)){
        cout<<"error when retriving page"<<endl;
        return false;
    }
    if(!Steg::hide(page,msg,host)){
        cout<<"error when hide the msg"<<endl;
        return false;
    }
	return true;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    string page;
    if(!HTTP().get(url, page)){
        cout<<"error when retriving page"<<endl;
        return false;
    }
    if(!Steg::reveal(page, msg)){
        cout<<"error when reveal the msg"<<endl;
        return false;
    }
	return true;  // This compiles, but may not be correct
}
