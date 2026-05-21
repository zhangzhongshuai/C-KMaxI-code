#include<bits/stdc++.h>

int MAX_DATA_SIZE = 1e7+10000,w,s,k=1000;

using namespace std;

long long create_crest = 0;
long long use_crest = 0;

long long memory_cnt=0;
long long maxx_crest_cnt=0;
long long now_cnt=0;

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
        ++use_crest;
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
                create_crest++;
                maxx_crest_cnt = max(maxx_crest_cnt,1ll*top_res+top_wait);
                min_s = current;
            }else {
                wait[top_wait++] = {i,i,current,current+input[i],top_wait-1};
                maxx_crest_cnt = max(maxx_crest_cnt,1ll*top_res+top_wait);
                create_crest++;
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
    memory_cnt+=maxx_crest_cnt;
    maxx_crest_cnt=0;
}

void DataInit_input(){
    vector<double> a(MAX_DATA_SIZE+1);
    a[0] = 0;
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

int cnt_window = 0;
int main(int argc, char* argv[])
{
    double W,S;
    if(argc<2){
        W=1;
        S=0.1;
        MAX_DATA_SIZE=10000000;
        choose_data(MAX_DATA_SIZE);
        w=MAX_DATA_SIZE*W*0.01;
        s=w*S*0.01;
        if(s<1) s=1;
        MAX_DATA_SIZE*=1;
        k=50; 
    }
    else{

        MAX_DATA_SIZE=std::atoi(argv[1]);
        W=std::atof(argv[2]);
        S=std::atof(argv[3]);
        w=MAX_DATA_SIZE*W*0.01;
        s=ceil(w*S*0.01);
        k=std::atoi(argv[4]);
        choose_data(MAX_DATA_SIZE);
    }
    RR = w-1;

    freopen("Memory_Candidate_RuzzoTompa.txt","a",stdout);
    DataInit_input();
    printf("n=%d s=%d w=%d k=%d    ",MAX_DATA_SIZE,s,w,k);
    Max_TopKHead = new TopKHeap();
    int cnt = 0;
    while (1)
    {
        run2();
        print();
        ++cnt_window;
        

        top_wait = 0;
        top_res = 0; 
        
        LL += s;
        RR += s;
        if(RR >= MAX_DATA_SIZE - 2){
            ++cnt_window;
            RR = MAX_DATA_SIZE - 2;
            run2();
            print();
            break;
        }
    }
//    printf("tot_sub_array=%lld\n",tot_sub_array);
//    printf("sizeof=%d\n",sizeof(sub_array));

    printf("average_candidate=%.4lf\n",tot_subarray_per_window/cnt_window);
    printf("average_identification_candidate_per_window=%.4lf\n",create_crest/cnt_window);
    printf("average_comparision_candidate_per_window=%.4lf\n",tot_subarray_per_window/cnt_window);
    printf("average_memory_usage_per_window=%/4lf",(memory_cnt*sizeof(sub_array))/cnt_window);
    return 0;
}
