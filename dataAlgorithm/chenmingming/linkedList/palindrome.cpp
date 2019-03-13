//
// Created by mingm on 2019/3/13.
//
#include <iostream>
#include <math.h>
using namespace std;
struct Node
{
    char data;
    Node* next;
    Node():next(NULL){}
    Node(char &ch):data(ch),next(NULL){}
    ~Node(){}
};
class SLinkedList
{
    Node* p_head;
public:
    SLinkedList(){ p_head = new Node; }
    ~SLinkedList(){ erase(); }
    void erase()
    {
        Node *del_tempNode, *tempNode;
        del_tempNode = p_head;
        while(del_tempNode != NULL) 
        {
            tempNode = del_tempNode -> next;
            delete del_tempNode;
            del_tempNode = tempNode;
        }
    }
    void set_head(Node* p)
    {
        p_head = p;
    }
    Node* get_head()
    {
        return p_head;
    }
    size_t get_len()
    {
        size_t len = 0;
        Node* p = p_head;
        while(p)
        {
            len++;
            p = p->next;
        }
        return len;
    }
    void insertAtEnd(char &data)
    {
        Node* newNode = new Node(data);
        static Node* tempNode = p_head;
        tempNode->next = newNode;
        tempNode = newNode;
    }
    void delHeadSentinel()
    {
        Node* del = p_head;
        p_head = p_head->next;
        delete del; //删除链表的表头哨兵
    }
    Node* reverse()
    {
        if(p_head == NULL || p_head->next == NULL)
            return NULL;
        else
        {
            Node *prevNode, *nextNode, *tempNode;
            prevNode = p_head;
            nextNode = prevNode->next;
            prevNode->next = NULL;
            while(nextNode != NULL)
            {
                tempNode = nextNode->next;
                nextNode->next = prevNode;
                prevNode = nextNode;
                nextNode = tempNode;
            }
            p_head = prevNode;
            return p_head;
        }
    }
    Node* findMiddle()
    {
        size_t len = get_len();
        Node* tempNode = p_head;
        size_t n = ceil(double(len)/2);
        for(size_t i = 1; i < n; ++i)
        {
            tempNode = tempNode->next;
        }
        return tempNode;
    }
};

int main()
{
    while(true)
    {
        char ch;
        cout << "enter a word, is it a palindrome ?" << endl;
        SLinkedList charList, backHalfOfList;
        while((ch = cin.get()) && ch != '\n')   //把单词存进链表
        {
            charList.insertAtEnd(ch);
        }
        charList.delHeadSentinel();
        Node* endOfFrontList = charList.findMiddle();
        Node* backListHead = endOfFrontList->next;
        endOfFrontList->next = NULL;
        backHalfOfList.set_head(backListHead);
        backHalfOfList.reverse();
        size_t n = backHalfOfList.get_len();
        Node *frontList = charList.get_head();
        Node *backList = backHalfOfList.get_head();
        bool answer = false;
        if(backList==NULL)
            answer = true;
        else
        {
            for(size_t i = 0; i < n; ++i)
            {
                if(frontList->data != backList->data)
                {
                    answer = false;
                    break;
                }
                else
                {
                    answer = true;
                    frontList = frontList->next;
                    backList = backList->next;
                }
            }
        }
        if(answer)
            cout << "the word is a palindrome." << endl;
        else
            cout << "the word is not a palindrome." << endl;
        char conti;
        cout << "continue to check? (y/n)" << endl;
        cin >> conti;
        if(conti == 'y' || conti == 'Y')
            continue;
        else
            break;
    }
    return 0;   
}