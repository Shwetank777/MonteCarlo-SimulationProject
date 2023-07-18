#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

//    Inputs:-
//        Enter no. of categories : 5     
//        Enter data of Category, Time Require, No of Patients : filling 45 40 crown 60 15 cleaning 15 15 extracting 45 10 checkup 15 20
//        Enter 8 Random Numbers: 40 82 11 34 25 66 17 79               
//        Enter Average Time and Starting Time: 30 8 00

vector<float> findProbability(vector<int> arr){
    int sum=0;
    for(auto x: arr){
        sum += x;
    }
    vector<float> vec;
    for(auto x: arr){
        float f=(float)x/sum;
        vec.push_back(round(f * 100) / 100);
    }
    return vec;
}

vector<float> findCumulative(vector<float> arr){
    float a=0.0;
    for(int i=0;i<arr.size();i++){
        arr[i]+=a;
        a=arr[i];
    }
    return arr;
}

vector<pair<int,int>> findRange(vector<float> arr){
    int a=1.0;
    vector<pair<int,int>> vec;
    for(int i=0;i<arr.size();i++){
        int b=arr[i]*100;
        vec.push_back(make_pair(a,b-1));
        a=b;
    }
    return vec;
}

vector<int> takeRandom(int n){
    vector<int> vec;
    cout<<"Enter "<<n<<" Random Numbers:"<<endl;
    for(int i=0;i<n;i++){
        int temp;
        cin>>temp;
        vec.push_back(temp);
    }
    return vec;
}

vector<pair<string,int>> get_service_time(vector<int> random, vector<int> time_require,vector<pair<int,int>> CPRange, vector<string>category){
    vector<pair<string,int>> vec;
    for(int i=0;i<random.size();i++){
        for(int j=0;j<CPRange.size();j++){
            if(random[i]>=CPRange[j].first && random[i]<=CPRange[j].second){
                vec.push_back(make_pair(category[j],time_require[j]));
                break;
            }
        }
    }
    return vec;
}



struct Time{
    int hour;
    int min;
    Time(){
        hour=0;
        min=0;
    }
};

vector<Time> ScheduledList(Time starttime,int avg_time,int x){
    Time temp;
    int inputhour=starttime.hour;
    int inputmin=starttime.min;
    vector<Time> vec;
    temp.hour=inputhour;
    temp.min=inputmin;
    vec.push_back(temp);
    for(int i=0;i<x-1;i++){
        if(avg_time>=60 || inputmin+avg_time>=60){
            inputhour++;
            inputmin=inputmin-(60-avg_time);
        }
        else{
            inputmin=inputmin+avg_time;
        }
        
        temp.hour=inputhour;
        temp.min=inputmin;
        vec.push_back(temp);
    }
    return vec;
}
int calcwait(Time schedule,Time actual){
    int actmin=0,schmin=0;
    actmin=actual.hour*60+actual.min;
    schmin=schedule.hour*60+schedule.min;
    if(actmin>schmin){
        return actmin-schmin;
    }
    return 0;
}

vector<Time> ActScheduledList(vector<Time> scheduledList,vector<pair<string ,int>> service,int x){

    Time temp;
    int inputhour=scheduledList[0].hour;
    int inputmin=scheduledList[0].min;
    vector<Time> vec;
    temp.hour=inputhour;
    temp.min=inputmin;
    vec.push_back(temp);
    int wait=0;
    for(int i=0;i<x-1;i++){
        inputhour=scheduledList[i].hour;
        inputmin=scheduledList[i].min;
        if(service[i].second+wait>=60 || inputmin+service[i].second+wait>=60){
            inputhour++;
            inputmin=inputmin-(60-service[i].second-wait);
        }
        else{
            inputmin=inputmin+service[i].second+wait;
        }
        temp.hour=inputhour;
        temp.min=inputmin;
        vec.push_back(temp);
        wait=calcwait(scheduledList[i+1],temp);
        }
        return vec;
}


vector<pair<int,int>> managetime(vector<Time> actual, vector<Time> schedule){
    int actmin=0,schmin=0;
    vector<pair<int,int>> vec;
    for(int i=0;i<schedule.size();i++){
        actmin=actual[i].hour*60+actual[i].min;
        schmin=schedule[i].hour*60+schedule[i].min;
        if(actmin==schmin){
            vec.push_back(make_pair(0,0));
        }
        else if(actmin>schmin){
            vec.push_back(make_pair(actmin-schmin,0));
        }
        else{
            vec.push_back(make_pair(0,schmin-actmin));
        }
    }
    return vec;
}



int main(){
    
    int n;
    vector<string> category;
    vector<int> time_req;
    vector<int> pat;
    cout<<"Enter no. of categories"<<endl;
    cin>>n;
    cout<<"Enter data of Category, Time Require, No of Patients"<<endl;

    // For Inputing Data
    for(int i=0;i<n;i++){
        string temp1;
        int temp2,temp3;
        cin>>temp1>>temp2>>temp3;
        category.push_back(temp1);
        time_req.push_back(temp2);
        pat.push_back(temp3);
    }

    vector<float> prob=findProbability(pat); // finding Probablities
    vector<float> CP=findCumulative(prob);  // finding Cumulative Probablities
    vector<pair<int,int>>CPRange=findRange(CP); // finding Range
    
    int x=8;
    vector<int> random=takeRandom(x); //take Randome Integers


    Time starttime;
    int avg_time;
    cout<<"Enter Average Time and Starting Time"<<endl;
    cin>>avg_time>>starttime.hour>>starttime.min;

    vector<Time> scheduledList=ScheduledList(starttime,avg_time,x); // scheduleList generated 

    vector<pair<string,int>> service=get_service_time(random,time_req,CPRange,category); // service contain {category, service time needed}
    
    vector<Time> act_schedule=ActScheduledList(scheduledList,service,x); // actual schedule generated

    vector<pair<int,int>> wait_ideal_time=managetime(act_schedule,scheduledList); // wait and ideal time generated



    // For Printing 
    cout<<"S.No."<<setw(20)<<"Category"<<setw(20)<<" Time Req"<<setw(20)<<"No. of People"<<setw(20)<<"Probablity"<<setw(20)<<" Cum. Prob"<<setw(20)<<"Range"<<endl;
    for(int i=0;i<n;i++){
        cout<<i+1<<setw(20)<<category[i]<<setw(20)<<time_req[i]<<setw(20)<<pat[i]<<setw(20)<<prob[i]<<setw(20)<<CP[i]<<setw(20)<<CPRange[i].first<<"-"<<CPRange[i].second<<endl;
    }

    cout<<"Random No."<<setw(20)<<"Schedule Time"<<setw(20)<<" Actual Schedule"<<setw(20)<<"Category"<<setw(20)<<"Service Time"<<setw(20)<<"Waiting Time"<<setw(20)<<"Idle Time"<<endl;
    for(int i=0;i<random.size();i++){
        cout<<random[i]<<setw(20)<<scheduledList[i].hour<<":"<<scheduledList[i].min<<setw(20)<<act_schedule[i].hour<<":"<<act_schedule[i].min<<setw(20)<<service[i].first<<setw(20)<<service[i].second<<setw(20)<<wait_ideal_time[i].first<<setw(20)<<wait_ideal_time[i].second<<endl;
    }

}