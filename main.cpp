#include <iostream>
#include<Windows.h>
#include <time.h>
#include <sstream>


using namespace std;

//array for lists
DWORD list [5];
DWORD trList [20];
int waiting [5];
time_t crTime[5];
string elTime[20];

string int_to_str(int x) {
   stringstream ss;
   ss << x;
   return ss.str();
}

//prints the list of processes
void printList(){
	cout<<"Current process list:"<<list[0]<<"-->"<<list[1]<<"-->"<<list[2]<<"-->"<<list[3]<<"-->"<<list[4]<<endl;
	cout<<"Requests in waiting list:"<<waiting[0]<<"-->"<<waiting[1]<<"-->"<<waiting[2]<<"-->"<<waiting[3]<<"-->"<<waiting[4]<<endl;
	cout<<"**************************************"<<endl;
}
void printTime(){
	int k=0;
	cout<<"Terminated Process List:"<<endl;
	while(trList[k]!=0){
		cout<<"(Process ID:"<<trList[k]<<"  ,  ";
		if(trList[k+1]!=0){
				cout<<"Time Duration:"<<elTime[k]<<")------>";
		}else{
			cout<<"Time Duration:"<<elTime[k]<<")";
		}
	
		k++;
	}
	cout<<endl;
	cout<<"**************************************"<<endl;
}

void printAll(){
	cout<<"process existance list:"<<list[0]<<"-->"<<list[1]<<"-->"<<list[2]<<"-->"<<list[3]<<"-->"<<list[4]<<"-->";
	int i;
	for(i = 0; i<20; i++){
		if (i!=19){
			cout<<trList[i]<<"-->";
		}else{
			cout<<trList[i]<<endl;
		}
		
	}
}

//process creation
void run(){
	
	HANDLE hProcess = NULL;
	HANDLE hThreads = NULL;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD dwProcessId = 0;
	DWORD dwThreadId = 0; 
	ZeroMemory (&si, sizeof(si));
	
	if (list[4]==0){//checks if the list is full or not
		BOOL bCreateProcess = CreateProcess("C:\\Windows\\System32\\notepad.exe",NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);	
	
		if(bCreateProcess == FALSE){
			cout<<"PROCESS CREATION FAILED AND ERROR - "<<GetLastError()<<endl;
		}else{
			cout<<"PROCESS CREATION SUCCESSFUL"<<endl;
		}
		
		//time stamp when process is created-------------------------------------
		time_t currentTime;
		time(&currentTime);
		struct tm *creTime = localtime(&currentTime);
		//-----------------------------------------------------------------------
		
	
		//Adding process ID to the list------------------------------------------
		int i=0;
		while(list[i]!=0){
			i++;
		}
		list[i]=pi.dwProcessId;
		crTime[i]=currentTime;
		//-----------------------------------------------------------------------
	
	
		//prints details of the process-------------------------------------------
		cout<<"PROCESS ID ->"<<pi.dwProcessId<<endl;
		cout<<"THREAD ID ->"<<pi.dwThreadId<<endl;
		cout<<"**************************************"<<endl;
		//------------------------------------------------------------------------
	}
		
}

//process termination
void kill(){ 
		printList();//printing list so that one could know the present processes in the list
		
		//entering process to be terminated---------------------------------------------------------------------
		cout<<"please enter process ID of process to be terminated from the above list just printed: "<<endl;
        DWORD pID;
        cin >> pID;
        //------------------------------------------------------------------------------------------------------
        
        HANDLE tmpHandle = OpenProcess(PROCESS_ALL_ACCESS,TRUE,pID);
        TerminateProcess(tmpHandle,0); 
        
        
        int i=0;
        int time_elapsed;
        bool isfound=false;//for checking if process is found
		
		time_t currentTime;
		time(&currentTime);
		struct tm *trTime = localtime(&currentTime);
			
		int hr_tr = (trTime->tm_hour);
		int min_tr = (trTime->tm_min);
		int sec_tr = (trTime->tm_sec);
		string elapsed_time;
		
		while(i<5 && pID!=0){
        	if(list[i]==pID){
        		isfound=true;
        		struct tm *myTime = localtime(&crTime[i]);
        	
        		int	hr_cr = (myTime->tm_hour);
				int	hr_el = hr_tr - hr_cr;
				if(hr_el<0){
					hr_el = hr_el * -1;
				}
				
				int min_cr = (myTime->tm_min);
				int min_el = min_tr - min_cr;
				if(min_el<0){
					min_el =min_el * -1;
				}
				
				int sec_cr = (myTime->tm_sec);
				int sec_el = sec_tr - sec_cr;
				if(sec_el<0){
					sec_el = sec_el * -1;
				}
				
				
				elapsed_time =int_to_str(hr_el) +":"+int_to_str(min_el)+":"+int_to_str(sec_el);


        		
        		//for deleting the process from the list and then shifting rest of the processes up the queue/list
				if(i==4){
					list[i]=0;
				}else{
					int j=i;
					while(j!=4){
						list[j]=list[j+1];
						j++;
					}
					list[j]=0;
				}
				//---------------------------------------------------
				
				int k=0;
				while(trList[k]!=0){
					k++;
				}
				trList[k]= pID;
				elTime[k]= elapsed_time;
					
			}
			i++;
		}
		
			//messages--------------------------------------------------------------------------
		if(isfound==false){
			cout<<"could not kill the process as process ID is not found"<<endl;
		}else{
			cout<<"Process="<<pID<< " has been terminated "<<endl;
			printList();
		}
		
		if(waiting[0]==1){
			run();
			int j=0;
				
				while(j!=4){
					waiting[j]=waiting[j+1];
					j++;
				}
				waiting[4]=0;

		}
		
	
			cout<<"**************************************"<<endl;
		//----------------------------------------------------------------------------------
}


int main(int argc, char** argv) {
	string command;
	cout<<"NOTE: value 0 means there is NO process ID in list. List limit is of 5 processes."<<endl;
	cout<<"1 in waiting list means a process creation request. 0 means no process creation request."<<endl;

	
	do{
		cout<<"What do you want to do?"<<endl;
		cout<<"Press c- process creation "<<endl;
		cout<<"Press t- process termination"<<endl;
		cout<<"Press p- printing process list"<<endl;
		cout<<"Press a- printing all processes that existed list"<<endl;
		cout<<"Press d- printing terminated processes list"<<endl;
		cout<<"Press e- exit program"<<endl;
		cin >> command;
		
		if (command == "c"){
			if(list[4]==0){
				run();
			}else{
				int i=0;
				while(waiting[i]!=0){
					i++;
				}
				waiting[i]=1;
				cout<<"The current process List is full. The Process Request is added to waiting list. Terminate a process so that the process from waiting list could enter current process list."<<endl;
			}
			
		}else if (command == "t"){
			if(list[0]!=0){
				kill();
			}else{
				cout<<"Cannot terminate as there are no processes."<<endl;
			}
		}else if (command == "p"){
			printList();
		}else if(command == "e"){
			cout<<"Exiting Program...."<<endl;
			cout<<"Thank you! See you Later"<<endl;
		}else if (command == "d"){
			printTime();
		}else if(command == "a"){
			printAll();
		}
		else{
			cout<<"Incorrect command. Retry."<<endl;
		}
	}
	while(command!="e");
	

	return 0;
}



