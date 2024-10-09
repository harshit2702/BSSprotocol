#include<iostream>
#include<vector>
#include<string>
#include<utility>

using namespace std;

int main(){

    int n; //number of process
    cin>>n;
    vector<vector<int>> time(n,vector<int>(n, 0));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout<<time[i][j]<<" ";
        }
        cout<<endl;
    }

    vector<vector<pair<string, vector<bool> >>> transaction(n);

    string str;
    int processid = -1;
    cin.ignore();
    while(getline(cin,str) && str != "exit"){
        if (str.find("begin process p") == 0 and str.length() == 16){
            processid = stoi(str.substr(15)); // Extract the process number
            if (processid < n){
                cout <<str << endl;
            }else{
                cout << "Invalid process id" << endl;
                processid = -1;
            }
        }else if(str.compare("end process") == 0 and processid != -1){
            cout <<str<< endl;
            processid = -1;
            
        }else if(str.substr(0,4) == "send" and processid != -1){
            time[processid][processid] += 1;
            str += " (";
            for (int i = 0; i < n; i++) {
                str += to_string(time[processid][i]);
            }
            str += ")";
            string msg = str.substr(5); // Extract "m1 (100)"
            size_t startPos = msg.find(' '); // Find the first space
            size_t endPos = msg.find(' '); // Find the space before the parenthesis

            if (startPos != string::npos && endPos != string::npos) {
                msg = msg.substr(0, endPos); // Extract "m1"
            }
            vector<bool> msgReceived(n, false);
            msgReceived[processid] = true;

            transaction[processid].emplace_back(msg, msgReceived);

            cout <<str<< endl;
        }
        else{
            cout << "Invalid input" << endl;
        }
        str = "";
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < transaction[i].size(); j++){
            cout<<transaction[i][j].first<<endl;
        }
    }




cout<<"hello world"<<endl;



return 0;
}