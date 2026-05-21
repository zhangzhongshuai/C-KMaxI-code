#include<bits/stdc++.h>

int MAX_DATA_SIZE = 1e7+10000,w,s,k=1000;

using namespace std;

struct HeapNode{
    double value;
    int l,r;
};

long long cnt = 0;

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


typedef struct {
    int start; 
    int end; 
    double L; 
    double R; 
    int point;
}sub_array;

int LL = 0;
int RR;
vector<double> input(MAX_DATA_SIZE);
vector<sub_array> wait(MAX_DATA_SIZE);
vector<sub_array> res(MAX_DATA_SIZE);
int top_wait = 0;
int top_res = 0;

int tot=0;
long long tot_sub_array = 0;
long long tot_subarray_per_window = 0;
void clear() { 
    tot_sub_array += top_wait;
    tot+=top_wait;
    while (top_wait != 0) {
        ++tot_subarray_per_window;
        res[top_res++] = wait[--top_wait];
        Max_TopKHead->cal_topk({res[top_res].R - res[top_res].L,res[top_res].start,res[top_res].end});
    }
}
void run2() { 
    double current = 0;
    double min_s = INT_MAX;
    for(int i = LL;i<=RR;i++) {
        if(input[i] > 0) {
            if(current<=min_s) {
                clear();
                wait[top_wait++] = {i,i,current,current+input[i],-1};
                min_s = current;
            }else {
                wait[top_wait++] = {i,i,current,current+input[i],top_wait-1};
                int p = top_wait - 2;
                while (p > -1) {
                    if(wait[p].L < wait[top_wait-1].L) {
                        if (wait[p].R < wait[top_wait-1].R) {
                            wait[p].end = i;
                            wait[p].R = current + input[i];
                            top_wait = p + 1;
                        }else{
                            wait[top_wait-1].point = p;
                            break;
                        }
                    }
                    p = wait[p].point;
                }
            }
        }
        current += input[i];
    }
    clear();
}

void print() {
    Max_TopKHead->siz = 0;
    cnt += top_res;
}

void DataInit_input(){
    vector<double> a(MAX_DATA_SIZE+1);
    for(int i=0;i<MAX_DATA_SIZE;++i){
        scanf("%lf",&input[i]);
    }
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
        MAX_DATA_SIZE=10000000;
        choose_data(MAX_DATA_SIZE);
        w=MAX_DATA_SIZE*W*0.01;
        s=w*S*0.01;
        MAX_DATA_SIZE*=1;
        if(s<1) s=1;
        k=50; 
    }
    else{
        MAX_DATA_SIZE=std::atoi(argv[1]);
        W=std::atof(argv[2]);
        S=std::atof(argv[3]);
        w=MAX_DATA_SIZE*W*0.01;
        s=ceil(w*S*0.01);
        k=std::atoi(argv[4]);
        percent=std::atof(argv[5]);
        choose_data(MAX_DATA_SIZE);
        MAX_DATA_SIZE*=percent;
        state = std::atoi(argv[6]);
    }
    if(state==1){
        freopen("Throughput_RuzzoTompa.txt","a",stdout);
        printf("n=%d s=%d w=%d k=%d    ",MAX_DATA_SIZE,s,w,k);
    }
    if(state==2){
        freopen("Overall_RuzzoTompa.txt","a",stdout);
        printf("precent=%d%%    ",(int)(percent*100.0));
    }
    DataInit_input();
    Max_TopKHead = new TopKHeap();


    auto start = std::chrono::high_resolution_clock::now();

    RR = w-1;
    int cnt = 0;
    while (1)
    {
        run2();
        print();
        

        top_wait = 0;
        top_res = 0; 
        
        LL += s;
        RR += s;
        if(RR >= MAX_DATA_SIZE - 2){
            RR = MAX_DATA_SIZE - 2;
            run2();
            print();
            break;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    printf("n=%d s=%lf%% w=%lf%% k=%d    ",MAX_DATA_SIZE,S,W,k);
    std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
    return 0;
}
