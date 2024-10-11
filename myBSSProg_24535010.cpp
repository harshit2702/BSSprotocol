#include<iostream>
#include<vector>
#include<string>
#include<utility>

using namespace std;

struct messages2{
        string msg;
        int sender;
        vector<bool> msgReceived;
        vector<int> time;

        messages2(const string& msg, int sender, const vector<bool>& msgReceived, const vector<int>& time) : msg(msg), sender(sender), msgReceived(msgReceived), time(time) {}

};

bool checkBSSRule(const vector<int>& msgTime, const vector<int>& recieverTime, int senderid){
    if(msgTime[senderid] == recieverTime[senderid] + 1){
        for(int i = 0; i < msgTime.size(); i++){
            if(i != senderid){
                if(msgTime[i] > recieverTime[i]){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

void recievetransaction(string str, vector<vector<int>>& time, vector<vector<messages2>>& transaction, vector<vector<string>>& buffer, int n, int& processid, vector<vector<string>>& output){
    cout<<"Recieved Transaction "<<str<<endl;
    int recievedFrom = stoi(str.substr(8)); // Extract the process number
    string msg = str.substr(10,str.size()-1);
    vector<int> recieverTime = time[processid];

    bool found = false;
    int msgsender;
    vector<bool> msgReceived;
    vector<int> msgTime;


    for (auto& trans : transaction[recievedFrom]) {
        if (trans.msg == msg) {
            found = true;
            msgsender = trans.sender;
            msgReceived = trans.msgReceived;
            msgTime = trans.time;

            if(recievedFrom == msgsender){
                if(msgReceived[processid] == false){
                    string temp = str;
                    temp += " (";
                    for (int i = 0; i < n; i++) {
                        temp += to_string(time[processid][i]);
                    }
                    temp += ")";
                    output[processid].push_back(temp);
                }
                msgReceived[processid] = true;
                trans.msgReceived = msgReceived;
                if(checkBSSRule(msgTime,recieverTime, msgsender)){
                    time[processid][msgsender] += 1;
                    msgReceived[processid] = true;
                    string temp = "recv_A p" + to_string(msgsender) + " " + msg + " (";
                    for (int i = 0; i < n; i++) {
                        temp += to_string(time[processid][i]);
                    }
                    temp += ")";
                    output[processid].push_back(temp);
                    if (buffer[processid].size() > 0) {

                        while(buffer[processid].empty() == false){
                            string str = buffer[processid].front();
                            buffer[processid].erase(buffer[processid].begin());
                            recievetransaction(str, time, transaction, buffer, n, processid, output);
                            if(buffer[processid].front() == str){
                                break;
                            }
                        }
                    }
                }else{
                    buffer[processid].push_back(str);
                }
            }

            break;
        }
    }

    if (!found) {
        buffer[processid].push_back(str);

    }
    return ;
}

void inputstring(string& str, vector<vector<int>>& time, vector<vector<messages2>>& transaction, vector<vector<string>>& buffer, int n, int& processid, vector<vector<string>>& output){
    cout<<"Input String "<<str<<endl;
    if (str.find("begin process p") == 0 and str.length() == 16){
            processid = stoi(str.substr(15)); // Extract the process number
            if (processid < n){
                output[processid].push_back(str);
            }else{
                cout << "Invalid process id" << endl;
                processid = -1;
            }
        }else if(str.compare("end process") == 0 and processid != -1){
            processid = -1;
            
        }else if(str.substr(0,4) == "send" and processid != -1){
            time[processid][processid] += 1;
            str += " (";
            for (int i = 0; i < n; i++) {
                str += to_string(time[processid][i]);
            }
            str += ")";
            string msg = str.substr(5); 
            size_t startPos = msg.find(' '); 
            size_t endPos = msg.find(' '); 

            if (startPos != string::npos && endPos != string::npos) {
                msg = msg.substr(0, endPos); 
            }
            vector<bool> msgReceived(n, false);
            msgReceived[processid] = true;

            vector<int> timeSent = time[processid];

            transaction[processid].emplace_back(msg, processid , msgReceived, timeSent);

            output[processid].push_back(str);
        }else if(str.substr(0,8) == "recv_B p" and processid != -1){
            recievetransaction(str, time, transaction, buffer, n, processid, output);
        }
        else{
            cout << "Invalid input" << endl;
        }
        str = "";
}
int main(){

    int n; //number of process
    cin>>n;
    vector<vector<int>> time(n,vector<int>(n, 0));
    cout<<"Initial Time Matrix"<<endl;
    for(int i = 0; i < n; i++){
        cout<<"Time for process "<<i<<" -> ";
        for(int j = 0; j < n; j++){
            cout<<time[i][j]<<" ";
        }
        cout<<endl;
    }
    

    vector<vector< messages2 >> transaction(n);
    vector<vector<string>> buffer(n);
    vector<vector<string>> output(n);


    string str;
    int processid = -1;
    cin.ignore();
    while(getline(cin,str) && str != "exit"){
        inputstring(str, time, transaction, buffer, n, processid, output);
    }
    for(int i = 0; i < n; i++){
        while(buffer[i].empty() == false){
            string str = buffer[i].front();
            buffer[i].erase(buffer[i].begin());
            recievetransaction(str, time, transaction, buffer, n, i, output);
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < output[i].size(); j++){
            cout<<output[i][j]<<endl;
        }
        cout<<"end process"<<endl;
    }


    cout<<"Final Time Matrix"<<endl;
    for(int i = 0; i < n; i++){
        cout<<"Time for process "<<i<<" -> ";
        for(int j = 0; j < n; j++){
            cout<<time[i][j]<<" ";
        }
        cout<<endl;
    }



    //print buffer 
    for(int i = 0; i < n; i++){
        for(int j = 0; j < buffer[i].size(); j++){
            cout<<"Buffer for process "<<i<<" remaining is ";
            cout<<buffer[i][j]<<endl;
        }
    }




cout<<"BSS Protocol Completed"<<endl;



return 0;
}