#include<bits/stdc++.h>
#define lson (x*2)
#define rson (x*2+1)

const int MAXN=1e7+100000;

using namespace std;
typedef struct Node{
    double L; 
    double R; 
    double val; 
    int idx_L; 
    int idx_R;
    int from; 
    int to;
    int val_from; 
    int val_to;
    bool is_hole = false; 
}node;
node Nodes[4000000+400];
int n = 12,w,s,k=1000;

long long cnt_used_node = 0;
struct HeapNode{
    double value;
    int l,r;
    HeapNode() : value(0.0), l(0), r(0) {}
    HeapNode(double v, int left, int right) : value(v), l(left), r(right) {}
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
TopKHeap* Max_TopKHead = new TopKHeap();





double  input[MAXN];
double  prefix[MAXN];



void build(int x,int l,int r) {
    ++cnt_used_node;
    if(l == r) {
        Nodes[x].L = prefix[l] - input[l];
        Nodes[x].R = prefix[l];
        Nodes[x].val = input[l];
        Nodes[x].from = l;
        Nodes[x].to = r;
        Nodes[x].val_from = l;
        Nodes[x].val_to = r;
        Nodes[x].idx_L = l;
        Nodes[x].idx_R = r;
        return;
    }

    int mid = (l + r - 1) / 2;
    build(lson,l,mid);
    build(rson,mid + 1,r);

    if (Nodes[lson].L >= Nodes[rson].L) {
        Nodes[x].L = Nodes[rson].L;
        Nodes[x].idx_L = Nodes[rson].idx_L;
    }else {
        Nodes[x].L = Nodes[lson].L;
        Nodes[x].idx_L = Nodes[lson].idx_L;
    }
    if (Nodes[lson].R >= Nodes[rson].R) {
        Nodes[x].R = Nodes[lson].R;
        Nodes[x].idx_R = Nodes[lson].idx_R;
    }else {
        Nodes[x].R = Nodes[rson].R;
        Nodes[x].idx_R = Nodes[rson].idx_R;
    }

    if (Nodes[lson].val>Nodes[rson].val||(Nodes[lson].val == Nodes[rson].val && Nodes[lson].val_to - Nodes[lson].val_from < Nodes[rson].val_to - Nodes[rson].val_from)) {
        Nodes[x].val = Nodes[lson].val;
        Nodes[x].val_from = Nodes[lson].val_from;
        Nodes[x].val_to = Nodes[lson].val_to;
    }else {
        Nodes[x].val = Nodes[rson].val;
        Nodes[x].val_from = Nodes[rson].val_from;
        Nodes[x].val_to = Nodes[rson].val_to;
    }
    if (Nodes[rson].R-Nodes[lson].L>Nodes[x].val||
        (Nodes[rson].R-Nodes[lson].L == Nodes[x].val && 
            (Nodes[rson].idx_R - Nodes[lson].idx_L < Nodes[x].val_to - Nodes[x].val_from||
                (Nodes[rson].idx_R - Nodes[lson].idx_L == Nodes[x].val_to - Nodes[x].val_from&&Nodes[lson].idx_L > Nodes[x].val_from)))) {
        Nodes[x].val = Nodes[rson].R-Nodes[lson].L;
        Nodes[x].val_from = Nodes[lson].idx_L;
        Nodes[x].val_to = Nodes[rson].idx_R;
    }

    Nodes[x].from = l;
    Nodes[x].to = r;

    //cout<<l<<' '<<r<<' '<<newNode->val<<endl;
}
void update(int x,int hole_l,int hole_r) {

    if(Nodes[x].from>hole_r||Nodes[x].to<hole_l) return;

    if(Nodes[x].from>=hole_l&&Nodes[x].to<=hole_r) {
        Nodes[x].is_hole = true;
        Nodes[x].val = INT_MIN;
        Nodes[x].L = INT_MAX;
        Nodes[x].R = INT_MIN;
        return;
    }

    if(Nodes[x].is_hole == true&&Nodes[x].val == INT_MIN&&Nodes[x].L == INT_MAX&&Nodes[x].R == INT_MIN) return;

    update(lson,hole_l,hole_r);
    update(rson,hole_l,hole_r);

    if(Nodes[rson].is_hole) Nodes[x].L = Nodes[rson].L,Nodes[x].idx_L = Nodes[rson].idx_L;
    else {

        if(Nodes[lson].L >= Nodes[rson].L) {
            Nodes[x].L = Nodes[rson].L;
            Nodes[x].idx_L = Nodes[rson].idx_L;
        }else {
            Nodes[x].L = Nodes[lson].L;
            Nodes[x].idx_L = Nodes[lson].idx_L;
        }
    }
    if(Nodes[lson].is_hole) Nodes[x].R = Nodes[lson].R,Nodes[x].idx_R = Nodes[lson].idx_R;
    else {

        if(Nodes[lson].R >= Nodes[rson].R) {
            Nodes[x].R = Nodes[lson].R;
            Nodes[x].idx_R = Nodes[lson].idx_R;
        }else {
            Nodes[x].R = Nodes[rson].R;
            Nodes[x].idx_R = Nodes[rson].idx_R;
        }
    }

    if(Nodes[lson].is_hole||Nodes[rson].is_hole) Nodes[x].is_hole = true;

    if (Nodes[lson].val>Nodes[rson].val||(Nodes[lson].val == Nodes[rson].val && Nodes[lson].val_to - Nodes[lson].val_from < Nodes[rson].val_to - Nodes[rson].val_from)) {
        Nodes[x].val = Nodes[lson].val;
        Nodes[x].val_from = Nodes[lson].val_from;
        Nodes[x].val_to = Nodes[lson].val_to;
    }else {
        Nodes[x].val = Nodes[rson].val;
        Nodes[x].val_from = Nodes[rson].val_from;
        Nodes[x].val_to = Nodes[rson].val_to;
    }
    if (Nodes[rson].R-Nodes[lson].L>Nodes[x].val||
        (Nodes[rson].R-Nodes[lson].L == Nodes[x].val && 
            (Nodes[rson].idx_R - Nodes[lson].idx_L < Nodes[x].val_to - Nodes[x].val_from||
                (Nodes[rson].idx_R - Nodes[lson].idx_L == Nodes[x].val_to - Nodes[x].val_from&&Nodes[lson].idx_L > Nodes[x].val_from)))) {
        Nodes[x].val = Nodes[rson].R-Nodes[lson].L;
        Nodes[x].val_from = Nodes[lson].idx_L;
        Nodes[x].val_to = Nodes[rson].idx_R;
    }
}

double a[MAXN];
void DataInit_input(){
    a[0] = 0;
    for(int i=0;i<n;++i){
        scanf("%lf",&input[i-1]);
    }
    for(int i = 0;i<n;++i){
        if(i == 0) prefix[i] = input[i];
        else prefix[i] = prefix[i-1] + input[i];
    }
}
int cnt_windows = 0;
void run2(){
    int LL = 0,RR = w-1;

    
    while (1)
    {
        ++cnt_windows;
        build(1,LL,RR);
        int cnt=0;
        while (Nodes[1].val>0) {
            HeapNode h;
            h.value=Nodes[1].val;
            h.l = Nodes[1].val_from;
            h.r = Nodes[1].val_to;
            Max_TopKHead->cal_topk(h);
            update(1,Nodes[1].val_from ,Nodes[1].val_to);
            ++cnt;
            if(cnt>k) break;
        }

        break;
        
        LL += s;
        RR += s;
        Max_TopKHead->siz = 0;
        if(RR >= n - 2){
            ++cnt_windows;
            RR = n - 2;
            build(1,LL,RR);
            while (Nodes[1].val>0) {
               HeapNode h;
                h.value=Nodes[1].val;
                h.l = Nodes[1].val_from;
                h.r = Nodes[1].val_to;
                Max_TopKHead->cal_topk(h);
                update(1,Nodes[1].val_from ,Nodes[1].val_to);
                ++cnt;
                if(cnt>k) break;
            }
            break;
        }
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
        choose_data(n);
    }
    freopen("Memory_Tournament.txt","a",stdout);

    DataInit_input();
    printf("n=%d s=%d w=%d k=%d    ",n,s,w,k);

    auto start = std::chrono::high_resolution_clock::now();

    run2();

    printf("average_memory_usage_per_window=%.4lf",cnt_used_node*sizeof(node));
    return 0;
}
