#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;
class cManager{
    public:
        int nodeID;
        string message;

        cManager(){
            message = "";
            nodeID = 0;
        }

        cManager(string nMessage, int nID){
            message = nMessage;
            nodeID = nID;
        }

        virtual int getID(){
            return nodeID;
        }

        virtual string getMessage(){
            return message;
        }

        virtual cManager startTimer(cManager n){
        int slot = 1;
        while(true)
        {
            cout << "slot #: " << slot << endl;
            cout << "node ID " << n.getID() <<  " has this message: "<< n.getMessage() << endl;
            slot++;
            Sleep(5000); //about 5 seconds
            if(slot == 5)
                break;

        }
        }

};

int main()
{
    vector<cManager> nItems;
    cManager node1("Hi I have a message", 1);
    cManager node2("Bye", 2);
    cManager node3("", 3);

    // put nodes in a vector
    nItems.push_back(node1);
    nItems.push_back(node2);
    nItems.push_back(node3);

    // one instance of checking node vector
    // if there is a message, print out node number and message
    //if no message, state node number and
    for(int i = 0; i < nItems.size(); ++i){
        if(!nItems[i].message.empty())
            cout << "node ID " << nItems[i].getID() <<  " has this message: "<< nItems[i].getMessage() << endl;
        else
            cout << "node ID " << nItems[i].nodeID << " has no message" << endl;
    }
    node1.startTimer(node1);

    return 0;
}
