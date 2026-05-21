#include<bits/stdc++.h>

const int MAX_DATA_SIZE = 1e7+10000;
int n,w,s,k=400;
int cnt=0;

using namespace std;

struct HeapNode{
    double value;
    int l,r;
};

class TopKHeap{
public:
    HeapNode* heap;
    int siz;
    TopKHeap(){
        heap = new HeapNode[k+1];
        siz = 0;
    }
    void Push(HeapNode x){
        int now = ++siz;
        while(now>1){
            int p = now/2;
            if(heap[p].value <= x.value) break;
            heap[now] = heap[p];
            now = p;
        }
        heap[now] = x;
    }
    HeapNode Pop(){
        HeapNode ans = heap[1];
        heap[1] = heap[siz];
        --siz;
        int now=1;
        while((now<<1)<=siz){
            int son = now<<1;
            if(son+1<=siz && heap[son+1].value<heap[son].value) ++son;
            if(heap[son].value>=heap[now].value) break;
            std::swap(heap[son],heap[now]);
            now = son;
        }
        return ans;
    }

    bool cal_topk(HeapNode x){
        if(siz<k) Push(x);
        else{
            if(x.value<=heap[1].value) return false;
            else{
                Pop();
                Push(x);
            }
        }
        return true; 
    }
};
TopKHeap* Max_TopKHead;

long long cnt_sub_array = 0;

long long tot_create = 0;
long long maxx_cnt_sub_array = 0;
//long long pre_cnt_sub_array = 0;
class sub_array{
public:
    int start; 
    int end; 
    double L; 
    double R; 
    int point;
    sub_array* pre;
    vector<sub_array*> nxt;
    sub_array(){
        pre = nullptr;
    }
    sub_array(int start,int end,double L,double R,sub_array* pre){
        this->start = start;
        this->end = end;
        this->L = L;
        this->R = R;
        this->pre = pre;
    }
};

struct segment{
    int start,end;
    double value;
};



int RR,LL=1;

class Partition{
public:
    int start=n+1,end=-1;
    std::deque<sub_array*> subs;
    bool last = false;
};

std::deque<Partition*> S_Max;

double input[MAX_DATA_SIZE];
double sum[MAX_DATA_SIZE];
double sum_L[MAX_DATA_SIZE];
double sum_R[MAX_DATA_SIZE];

int top_wait = 0;
int top_res = 0;

vector<sub_array*> tmp_wait;
vector<sub_array*> tmp_store;
int tmp_top_wait = 0;
int tmp_top_res = 0;
int tmp_store_siz = 0;

void clear(Partition* now,bool last) { 
    bool is_old_partition = now->last;
    now->last = last;
    for(int j=0;j<=now->subs.size()-1;++j){
        sub_array* cur = now->subs.at(j);
        now->end = max(now->end,cur->end);
        now->start = min(now->start,cur->start);
    }
    if(!is_old_partition){
        S_Max.push_back(now);
    }
    top_wait = 0;
}


double min_s = INT_MAX;
void run2(int L,int R){ 
    Partition* nowPartition;
    if(!S_Max.empty() && S_Max.back()->last){
        nowPartition  = S_Max.back();
        top_wait = nowPartition->subs.size();
    }
    else{
        nowPartition = new Partition();
        top_wait = 0;
    }

    for(int i = L;i<=R;i++) {
        if(input[i] > 0) {
            if(sum_L[i]<=min_s) { 
                if(top_wait>0){
                    clear(nowPartition,false);
                    nowPartition = new Partition();
                }
                sub_array* now = new sub_array(i,i,sum_L[i],sum_L[i]+input[i],nullptr);
                nowPartition->subs.push_back(now);
                top_wait++;
                min_s = sum_L[i];
            }else {
                sub_array* pre_one = nullptr;
                if(!nowPartition->subs.empty()) pre_one = nowPartition->subs.back();
                sub_array* now = new sub_array(i,i,sum_L[i],sum_L[i]+input[i],pre_one);
                if(pre_one!=nullptr) pre_one->nxt.push_back(now);
                nowPartition->subs.push_back(now);
                top_wait++;
                sub_array* p = pre_one;
                while (p != nullptr) {
                    if(p->L < nowPartition->subs[top_wait-1]->L) {
                        if (p->R < nowPartition->subs[top_wait-1]->R) {
                            p->end = i;
                            p->R = sum_L[i] + input[i];


                            while(p->start < nowPartition->subs.back()->start){
                                sub_array* cur = nowPartition->subs.back();
                                if(cur->pre!=nullptr){
                                    cur->pre->nxt.pop_back();
                                }
                                delete nowPartition->subs.back();
                                nowPartition->subs.pop_back();
                                top_wait--;
                                cnt_sub_array--;
                            }
                            
                        }else{
                            sub_array* old_pre = nowPartition->subs[top_wait-1]->pre;
                            nowPartition->subs[top_wait-1]->pre = p;
                            if(old_pre!=nullptr) old_pre->nxt.pop_back();
                            p->nxt.push_back(nowPartition->subs[top_wait-1]);
                            break;
                        }
                    }
                    p = p->pre;
                }
            }
        }
    }
    if(top_wait>0) clear(nowPartition,true);
}

void Rebuild_clear() { 
    if(!tmp_wait.empty()){
        for(int j=0;j<=tmp_wait.size()-1;++j){
            sub_array* cur = tmp_wait.at(j);
            tmp_store.push_back(cur);
         }
    }
    tmp_wait.clear();
    tmp_top_wait = 0;
}


void ReBuild_run2(int L,int R){ 
    double tmp_min_s = INT_MAX;
    tmp_top_wait = 0;
    tmp_store_siz = 0;
    tmp_wait.clear();
    tmp_store.clear();
    for(int i = L;i<=R;i++) {
        if(input[i] > 0) {
            if(sum_L[i]<=tmp_min_s) { 
                Rebuild_clear();
                sub_array* now = new sub_array(i,i,sum_L[i],sum_L[i]+input[i],/*-1*/nullptr);
                tmp_wait.push_back(now);
                tmp_top_wait++;
                tmp_min_s = sum_L[i];
            }else{


                sub_array* pre_one = nullptr;
                if(!tmp_wait.empty()) pre_one = tmp_wait.back();
                sub_array* now = new sub_array(i,i,sum_L[i],sum_L[i]+input[i],pre_one);
                if(pre_one!=nullptr) pre_one->nxt.push_back(now);
                tmp_wait.push_back(now);
                tmp_top_wait++;
                sub_array* p = pre_one;


                while (p != nullptr) {
                    if(p->L < tmp_wait[tmp_top_wait-1]->L) {
                        if (p->R < tmp_wait[tmp_top_wait-1]->R) {
                            p->end = i;
                            p->R = sum_L[i] + input[i];
                            
                            while(p->start < tmp_wait.back()->start){
                                sub_array* cur = tmp_wait.back();
                                if(cur->pre!=nullptr){
                                    cur->pre->nxt.pop_back();
                                }
                                delete tmp_wait.back();
                                tmp_wait.pop_back();
                                tmp_top_wait--;
                                cnt_sub_array--;
                            }
                            
                        }else{
                            sub_array* old_pre = tmp_wait[tmp_top_wait-1]->pre;
                            tmp_wait[tmp_top_wait-1]->pre = p;
                            old_pre->nxt.pop_back();
                            p->nxt.push_back(tmp_wait[tmp_top_wait-1]);
                            break;
                        }
                    }
                    p = p->pre;
                }
            }
        }
    }
    Rebuild_clear();
    
}

void cut_link(sub_array* cur){
    if(cur->nxt.empty()) return;
    sub_array* old_nxt;
    for(int i=0;i<=cur->nxt.size()-1;++i){
        old_nxt = cur->nxt.at(i);
        old_nxt->pre = nullptr;
    }
}


void Rebuild_sub(Partition* now,int r){
    tmp_store.clear();
    int L=r+1;
    int R=now->subs.front()->end;
    now->start = r+1;
    sub_array* cur = now->subs.front();
    now->subs.pop_front();

    ReBuild_run2(L,R);

    if(!tmp_store.empty()){
        if(!cur->nxt.empty()){
            for(int i=0;i<=cur->nxt.size()-1;++i){
                bool judge = false;
                for(int j=tmp_store.size()-1;j>=0;--j){
                    sub_array* right = cur->nxt.at(i);
                    sub_array* left = tmp_store.at(j);
                    if(left->L < right->L){
                        right->pre = left;
                        left->nxt.push_back(right);
                        judge = true;
                        break;
                    }
                }
                if(!judge){
                    cur->nxt.at(i)->pre = nullptr;
                }
            }
        }
        for(int i=tmp_store.size()-1;i>=0;--i){
            now->subs.push_front(tmp_store.at(i));
        }      
    }
    if(!cur->nxt.empty()){
        for(int i=0;i<=cur->nxt.size()-1;++i){
            if(cur->nxt.at(i)->pre == nullptr){
                auto it = std::find(now->subs.begin(),now->subs.end(),cur->nxt.at(i));
                if(it!=now->subs.begin()){
                    --it;
                    sub_array* it_pre = *it;
                    it_pre->nxt.push_back(cur->nxt.at(i));
                    cur->nxt.at(i)->pre = it_pre;
                }
            }
        }
    }
}

void Modify_subs(Partition* now,int r){
    int siz=now->subs.size()-1;
    for(int i=0;i<=siz;++i){
        if(now->subs.front()->end <= r){
            cut_link(now->subs.front());
            cnt_sub_array--;
            delete now->subs.front();
            now->subs.pop_front();
        }
        else{
            if(now->subs.front()->start <= r){
                Rebuild_sub(now,r);
                break;
            }
        }
    }
}

void Modify(int r){
    int siz = S_Max.size()-1;
    for(int i=0;i<=siz;++i){
        Partition* now = S_Max.front();
        if(now->end <= r){
            
            cnt_sub_array -= now->subs.size();

            delete S_Max.front();
            S_Max.pop_front();
        }
        else{
            if(now->start<=r){
                now->start = r+1;
                Modify_subs(now,r);            
            }
            break;
        }
    }
}   

void Sliding(int L,int R){
    Modify(L-1);
    run2(R-s+1,R);
}

bool cmp(segment x,segment y){
    return x.start < y.start;
}
bool cmp_value(segment x,segment y){
    return x.value > y.value;
}
segment res[MAX_DATA_SIZE];

long long tot_subarray_per_window = 0;

void print(){
    tot_subarray_per_window += maxx_cnt_sub_array;
    maxx_cnt_sub_array = 0;
    int tot=0;
    TopKHeap* topkheap = new TopKHeap();
    if(!S_Max.empty()){
        for(int i=0;i<=S_Max.size()-1;++i){
            Partition* now = S_Max.at(i);
            if(!now->subs.empty()){
                for(int j=0;j<=now->subs.size()-1;++j){
                    sub_array* x = now->subs.at(j);
                    topkheap->cal_topk((HeapNode){x->R - x->L,x->start,x->end});
                }
            }

        }
    }
}

double a[MAX_DATA_SIZE];


void DataInit_input(){

    for(int i=1;i<=n+1;++i){
        scanf("%lf",&input[i]);
    }
}
void getSum(){
    for(int i=1;i<=n;++i) sum_R[i] = sum_R[i-1] + input[i];
    for(int i=1;i<=n;++i) sum_L[i] = sum_R[i] - input[i];
}

void Stock_DataInit_input(){freopen("Stock.txt","r",stdin);}
void Genome_DataInit_input(){freopen("Genome.txt","r",stdin);}
void Normal_DataInit_input(){freopen("Normal.txt","r",stdin);}
void Uniform_DataInit_input(){freopen("Uniform.txt","r",stdin);}
void Power_DataInit_input(){freopen("Power.txt","r",stdin);}


void choose_data(int n){
    if(n==9028997) Stock_DataInit_input();
    if(n==10000000) Normal_DataInit_input();
    if(n==9999999) Uniform_DataInit_input();    
    if(n==1837622) Genome_DataInit_input();   
    if(n==246284) Power_DataInit_input();
}


int main(int argc, char* argv[])
{
    double W,S;
    double percent=1;
    int state=1;
    if(argc<2){
        W=1;
        S=0.1;
        n=10000000;
        choose_data(n);
        w=n*W*0.01;
        s=w*S*0.01;
        n*=1;
        if(s<1) s=1;
        k=50; 
    }
    else{

        n=std::atoi(argv[1]);
        W=std::atof(argv[2]);
        S=std::atof(argv[3]);
        w=n*W*0.01;
        s=ceil(w*S*0.01);
        k=std::atoi(argv[4]);
        percent=std::atof(argv[5]);
        choose_data(n);
        n*=percent;
        state = std::atoi(argv[6]);
    }
    if(state==1){
        freopen("Throughput_Streaming_RT.txt","a",stdout);
        printf("n=%d s=%d w=%d k=%d    ",n,s,w,k);
    }
    if(state==2){
        freopen("Overall_Streaming_RT.txt","a",stdout);
        printf("precent=%d%%    ",(int)(percent*100.0));
    }
    RR = w;

    
    Max_TopKHead = new TopKHeap();
    DataInit_input();


    auto start = std::chrono::high_resolution_clock::now();
    getSum();
    

    run2(LL,RR);
    print();
    
    
    LL += s;
    RR += s;

    

    while (1)
    {
        ++cnt;
//        printf("AAAAAAAAAA\n");
//        printf("L=%d R=%d\n",LL,RR);
//        if(cnt>=1000) break;
        Sliding(LL,RR);
        print();
        
//        if(cnt==4) return 0;

        top_wait = 0;
        top_res = 0; 
        
        LL += s;
        RR += s;
        if(RR >= n){
            RR = n;
            Sliding(LL,RR);
            print();
            break;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    printf("n=%d s=%lf%% w=%lf%% k=%d    ",n,S,W,k);
    std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
    return 0;
}
/*
L=63
L=159
L=145
L=142
L=121
L=63
L=159
L=145
L=142
L=121
L=63
*/