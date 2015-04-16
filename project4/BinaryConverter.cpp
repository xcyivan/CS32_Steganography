#include "provided.h"
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers){
	string res="";
    for(int i=0; i<numbers.size(); i++){
        string s=convertNumberToBitString(numbers[i]);
        for(int j=0; j<s.size(); j++){
            if(s[j]=='1') res+="\t";
            if(s[j]=='0') res+=" ";
//            if(s[j]=='1') res+="-";
//            if(s[j]=='0') res+="_";
        }
    }
    return res;  // This compiles, but may not be correct
}

bool BinaryConverter::decode(const string& bitString, vector<unsigned short>& numbers){
    if(bitString.size()%BITS_PER_UNSIGNED_SHORT!=0)    return false;
    for(int i=0; 16*i<bitString.size(); i++){
        string s=bitString.substr(16*i,16);
        string ss="";
        for(int j=0; j<16; j++){
            if(s[j]=='\t') ss+='1';
            else if(s[j]==' ')  ss+='0';
//            if(s[j]=='-') ss+='1';
//            else if(s[j]=='_') ss+='0';
            else ss+=s[j];
        }
        unsigned short res=0;
        if (!convertBitStringToNumber(ss, res))    {return false;}
        else    {numbers.push_back(res);}
    }
	return true;  // This compiles, but may not be correct
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
