//
//  HashTable.h
//  project4
//
//  Created by xiacheyun on 3/6/15.
//  Copyright (c) 2015 xiacheyun. All rights reserved.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
using namespace std;

template <typename KeyType, typename ValueType>
class HashTable{
public:
    HashTable(unsigned int numBuckets, unsigned int capacity)
    : m_numBuckets(numBuckets), m_capacity(0), max_capacity(capacity),
    m_queueHead(nullptr), m_queueTail(nullptr){
        m_table = new Node*[m_numBuckets];
        for(int i=0; i<m_numBuckets; i++)   m_table[i]=nullptr;
    }
    ~HashTable(){delete m_table;}
    bool isFull() const{return m_capacity>=max_capacity;}
    bool set(const KeyType& key, const ValueType& value, bool permanent = false);
    bool get(const KeyType& key, ValueType& value) const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key, ValueType& value);
//    void dump();
//    void timely_dump();
    
private:
    // We prevent a HashTable from being copied or assigned by declaring the
    // copy constructor and assignment operator private and not implementing them.
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
    struct Node{
        KeyType m_key;
        ValueType m_value;
        bool m_permanent;
        Node* next;
        Node* prev;
        Node* queueNext;
        Node* queuePrev;
    };
    Node** m_table; // this is a pointer to the array of Nodes.
    int m_numBuckets;
    int m_capacity;
    int max_capacity;
    Node* m_queueHead;
    Node* m_queueTail;
};

unsigned int computeHash(const unsigned short key);
unsigned int computeHash(const string key);

//===========================================================================================
//inline implementation of computeHash()
//this function only convert a given type variable to unsigned int type
//===========================================================================================
inline
unsigned int computeHash(const unsigned short key){
    unsigned int kk=key;
    return kk;
}
inline
unsigned int computeHash(const string key){
    int total = 0;
    for(int i=0; i<key.size(); i++)
        total = (total<<5)+key[i];
    return total;
}
//this one does nothing only to avoid compile error
template <typename KeyType>
unsigned int computeHash(KeyType key){
    return 0;
}
//===========================================================================================
//inline implementation of HashTable::set() function
//===========================================================================================
template <typename KeyType, typename ValueType>
bool HashTable<KeyType,ValueType>::set(const KeyType& key, const ValueType& value, bool permanent){
    unsigned int computeHash(KeyType); // prototype
    unsigned int hashRes = computeHash(key);
    //cout<<"the hash result: "<<hashRes<<endl;
    int bucketNum = hashRes%m_numBuckets;
    //cout<<"the bucketNum: "<<bucketNum<<endl;
    Node* temp = m_table[bucketNum];
    //the bucket is not written
    if(temp==nullptr){
        if(isFull())	{return false;}
        //add to this cell
        else{
            //horizontally
            m_capacity++;
            Node* newNode = new Node;
            newNode->m_key=key;
            newNode->m_value=value;
            newNode->prev=nullptr;
            newNode->next=nullptr;
            m_table[bucketNum]=newNode;
            //vertically
            if(!permanent){
                if(m_queueHead==nullptr){
                    m_queueHead=newNode;
                    m_queueTail=newNode;
                    newNode->queuePrev=nullptr;
                    newNode->queueNext=nullptr;
                }
                else{
                    m_queueTail->queueNext=newNode;
                    newNode->queuePrev=m_queueTail;
                    m_queueTail=newNode;
                    m_queueTail->queueNext=nullptr;
                }
            }
            else
                newNode->m_permanent=true;
            return true;
        }
    }
    else{
        //if the key exits in table, replace it
        //=====================================
        while(temp->next!=nullptr){
            cout<<"temp->m_key: "<<temp->m_key<<endl;
            if(temp->m_key==key){
                cout<<"temp->m_key: "<<temp->m_key<<endl;
                temp->m_value=value;
                //vertically
                touch(key);
                return true;
            }
            temp=temp->next;
        }
        //check the last one
        if(temp->m_key==key){
            cout<<"temp->m_key: "<<temp->m_key<<endl;
            temp->m_value=value;
            //vertically
            touch(key);
            return true;
        }
        //=====================================
        //else if the key doesn't exit in table
        if(isFull())	{return false;}
        //add to tail
        else{
            //horizontally
            m_capacity++;
            Node* newNode = new Node;
            newNode->m_key=key;
            newNode->m_value=value;
            newNode->prev=temp;
            temp->next=newNode;
            newNode->next=nullptr;
            //vertically
            if(!permanent){
                if(m_queueHead==nullptr){
                    m_queueHead=newNode;
                    m_queueTail=newNode;
                    newNode->queuePrev=nullptr;
                    newNode->queueNext=nullptr;
                }
                else{
                    m_queueTail->queueNext=newNode;
                    newNode->queuePrev=m_queueTail;
                    m_queueTail=newNode;
                    newNode->queueNext=nullptr;
                }
            }
            else
                newNode->m_permanent=true;
            return true;
        }
    }
    return false;
}
//========================================================================================
//inline implementation of HashTable::get() function
//========================================================================================
template <typename KeyType, typename ValueType>
bool HashTable<KeyType,ValueType>::get(const KeyType& key, ValueType& value) const{
    unsigned int computeHash(KeyType); // prototype
    unsigned int hashRes = computeHash(key);
    int bucketNum = hashRes%m_numBuckets;
    Node* temp = m_table[bucketNum];
    while(temp){
        if(temp->m_key==key) {value=temp->m_value;return true;}
        temp=temp->next;
    }
    return false;
}
//========================================================================================
//inline implementation of HashTable::touch() function
//========================================================================================
template <typename KeyType, typename ValueType>
bool HashTable<KeyType,ValueType>::touch(const KeyType& key){
//    Node* temp=m_queueHead;
//    bool flag=false;
//    while(temp){
//        if(temp->m_key==key) {flag=true; break;}
//        else temp=temp->queueNext;
//    }
    bool flag=false;
    unsigned int computeHash(KeyType); // prototype
    unsigned int hashRes = computeHash(key);
    int bucketNum = hashRes%m_numBuckets;
    Node* temp = m_table[bucketNum];
    while(temp){
        if(temp->m_key==key) {flag=true;break;}
        temp=temp->next;
    }

    //if key is not found
    if(!flag || temp->m_permanent==true) return false;
    //if key is found at tail, do nothing
    if(temp==m_queueTail) return true;
    //else if key is found at head, discuss
    else if(temp==m_queueHead){
        //if there is only 1 node, do nothing
        if(m_queueHead==m_queueTail) return true;
        //else there are more than 1 node
        else{
            m_queueHead->queuePrev=m_queueTail;
            m_queueTail->queueNext=m_queueHead;
            m_queueTail=m_queueHead;
            m_queueHead=m_queueHead->queueNext;
            m_queueTail=temp;
            m_queueHead->queuePrev=nullptr;
            m_queueTail->queueNext=nullptr;
        }
    }
    //else the key is found at the middle
    else{
        temp->queuePrev->queueNext=temp->queueNext;
        temp->queueNext->queuePrev=temp->queuePrev;
        temp->queuePrev=m_queueTail;
        temp->queueNext=nullptr;
        m_queueTail->queueNext=temp;
        m_queueTail=temp;
    }
    return false;
}

//========================================================================================
//inline implementation of HashTable::discard() function
//========================================================================================
template <typename KeyType, typename ValueType>
bool HashTable<KeyType,ValueType>::discard(KeyType& key, ValueType& value){
    unsigned int computeHash(KeyType); // prototype
    //if no node to discard
    if(m_queueHead==nullptr)    return false;
    //if only 1 node
    if(m_queueHead==m_queueTail){
        //vertically
        Node* toDelete=m_queueHead;
        m_queueHead=nullptr;
        m_queueTail=nullptr;
        key=toDelete->m_key;
        value=toDelete->m_value;
        //horizontally, set to nullptr
        m_table[(computeHash(key)%m_numBuckets)]=nullptr;
        return true;
    }
    //if more than 1 node
    else{
        //vertically
        Node* toDelete=m_queueHead;
        m_queueHead->queueNext->queuePrev=nullptr;
        m_queueHead=m_queueHead->queueNext;
        key=toDelete->m_key;
        value=toDelete->m_value;
        //horizontally
        if(toDelete->prev==nullptr){
            if(toDelete->next==nullptr)
                m_table[(computeHash(key)%m_numBuckets)]=nullptr;
            else
                m_table[(computeHash(key)%m_numBuckets)]=toDelete->next;
            }
        else    toDelete->prev->next=toDelete->next;
        delete toDelete;
        return true;
    }
    return false;
}

//========================================================================================
//inline implementation of dump() for debug purpose
//========================================================================================
//template <typename KeyType, typename ValueType>
//void HashTable<KeyType,ValueType>::dump(){
//    for(int i=0; i<m_numBuckets; i++){
//        cout<<"["<<i<<"] : ";
//        Node* temp = m_table[i];
//        if(temp==nullptr)
//            cout<<"nullptr"<<endl;
//        else{
//            while(temp!=nullptr){
//                cout<<temp->m_value<<" ";
//                temp=temp->next;
//            }
//            cout<<endl;
//        }
//    }
//}
//========================================================================================
//inline implementation of timely_dump() for debug purpose
//========================================================================================
//template <typename KeyType, typename ValueType>
//void HashTable<KeyType,ValueType>::timely_dump(){
//    Node* temp=m_queueHead;
//    while(temp){
//        cout<<temp->m_value<<endl;
//        temp=temp->queueNext;
//    }
//    cout<<"the head is ";
//    if(m_queueHead==nullptr)    cout<<"nullptr"<<endl;
//    else    cout<<m_queueHead->m_value<<endl;
//    cout<<"the tail is ";
//    if(m_queueTail==nullptr)    cout<<"nullptr"<<endl;
//    else    cout<<m_queueTail->m_value<<endl;
//}

#endif