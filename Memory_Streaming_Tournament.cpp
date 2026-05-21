#include <bits/stdc++.h>

long long cnt_windows = 0;
long long tot_node_cnt_per_window = 0;
long long cnt_node = 0;
using namespace std;
int k;
class node{
public:
    int seg_L;
    int seg_R;
    double L; 
    double R; 
    double val; 
    node *lc = nullptr;
    node *rc = nullptr; 
    int idx_L; 
    int idx_R;
    int from;
    int to;
    int val_from; 
    int val_to;
    bool is_hole = false; 
    node(){
        ++cnt_node;
    }
};

struct Subsegment
{
    int l,r;
    double val;
}ans[1000000];
int tot=0;
bool cmp_pos_l(Subsegment a,Subsegment b){
    return a.l < b.l;
}
void print_ans(){
    std::sort(ans+1,ans+tot+1,cmp_pos_l);
    for(int i=1;i<=tot;++i){
        long long x = ans[i].val;
        printf("val=%lld\n",x);
//        printf("val=%lld l=%d r=%d\n",x,ans[i].l,ans[i].r);
    }
    printf("cnt_ans=%d\n",tot);
    tot = 0;
}


int n,w,s;
vector<double> input;
vector<double> prefix;
vector<double> A;

void Delete_Node(node* now){
    if(now->lc!=nullptr){
        Delete_Node(now->lc);
    }
    if(now->rc!=nullptr){
        Delete_Node(now->rc);
    }
    --cnt_node;
    delete now;
}

void update_L_R(node* &newNode){
    node* tmp1 = newNode->lc;
    node* tmp2 = newNode->rc;
    if (tmp1->L >= tmp2->L) {
        newNode->L = tmp2->L;
        newNode->idx_L = tmp2->idx_L;
    }else {
        newNode->L = tmp1->L;
        newNode->idx_L = tmp1->idx_L;
    }
    if (tmp1->R >= tmp2->R) {
        newNode->R = tmp1->R;
        newNode->idx_R = tmp1->idx_R;
    }else {
        newNode->R = tmp2->R;
        newNode->idx_R = tmp2->idx_R;
    }
}

void update_val(node* &newNode){
    node* tmp1 = newNode->lc;
    node* tmp2 = newNode->rc;
    if (tmp1->val>tmp2->val||(tmp1->val == tmp2->val && tmp1->val_to - tmp1->val_from < tmp2->val_to - tmp2->val_from)) {
        newNode->val = tmp1->val;
        newNode->val_from = tmp1->val_from;
        newNode->val_to = tmp1->val_to;
    }else {
        newNode->val = tmp2->val;
        newNode->val_from = tmp2->val_from;
        newNode->val_to = tmp2->val_to;
    }
    if (tmp2->R-tmp1->L>newNode->val||
        (tmp2->R-tmp1->L == newNode->val && 
            (tmp2->idx_R - tmp1->idx_L < newNode->val_to - newNode->val_from||
                (tmp2->idx_R - tmp1->idx_L == newNode->val_to - newNode->val_from&&tmp1->idx_L > newNode->val_from)))) {
        newNode->val = tmp2->R-tmp1->L;
        newNode->val_from = tmp1->idx_L;
        newNode->val_to = tmp2->idx_R;
    }
}

void setNode(node* tmp,int l,int r){
    tmp->seg_L = l;
    tmp->seg_R = r;
    tmp->L = prefix[l] - input[l];
    tmp->R = prefix[l];
    tmp->val = input[l];
    tmp->from = l;
    tmp->to = r;
    tmp->val_from = l;
    tmp->val_to = r;
    tmp->idx_L = l;
    tmp->idx_R = r;
}

void reBuild(node* tmp){
    tmp->is_hole = false;
    if(tmp->seg_L==tmp->seg_R){
        setNode(tmp,tmp->seg_L,tmp->seg_L);
        return;
    }
    if(tmp->lc->is_hole) reBuild(tmp->lc);
    if(tmp->rc->is_hole) reBuild(tmp->rc);
    update_L_R(tmp);
    
    update_val(tmp);
}

node* build(int l,int r) {
    if(l == r) {
        //node tmp = {l,r,input[l],nullptr,nullptr};
        node *tmp = new node();
        setNode(tmp,l,r);
        return tmp;
    }
    int mid = (l + r - 1) / 2;
    node *tmp1 = build(l,mid);
    node *tmp2 = build(mid + 1,r);
    
    node* newNode = new node();
    newNode->seg_L = l;
    newNode->seg_R = r;

    newNode->lc = tmp1;
    newNode->rc = tmp2;
    newNode->from = l;
    newNode->to = r;

    update_L_R(newNode);
    
    update_val(newNode);    

    //cout<<l<<' '<<r<<' '<<newNode->val<<endl;
    return newNode;
}
void update(node *n,int hole_l,int hole_r) {
    if(n->from>hole_r||n->to<hole_l) return;
    if(n->from>=hole_l&&n->to<=hole_r) {
        n->is_hole = true;
        n->val = INT_MIN;
        n->L = INT_MAX;
        n->R = INT_MIN;
        return;
    }
    if(n->is_hole == true&&n->val == INT_MIN&&n->L == INT_MAX&&n->R == INT_MIN) return;
    update(n->lc,hole_l,hole_r);
    update(n->rc,hole_l,hole_r);
    if(n->rc->is_hole) n->L = n->rc->L,n->idx_L = n->rc->idx_L;
    else {
        //n->L = min(n->rc->L,n->lc->L);
        if(n->lc->L >= n->rc->L) {
            n->L = n->rc->L;
            n->idx_L = n->rc->idx_L;
        }else {
            n->L = n->lc->L;
            n->idx_L = n->lc->idx_L;
        }
    }
    if(n->lc->is_hole) n->R = n->lc->R,n->idx_R = n->lc->idx_R;
    else {
        //n->R = max(n->rc->R,n->lc->R);
        if(n->lc->R >= n->rc->R) {
            n->R = n->lc->R;
            n->idx_R = n->lc->idx_R;
        }else {
            n->R = n->rc->R;
            n->idx_R = n->rc->idx_R;
        }
    }
    if(n->lc->is_hole||n->rc->is_hole) n->is_hole = true;
    update_val(n);
    //cout<<n->from<<' '<<n->to<<' '<<n->idx_L<<' '<<n->idx_R<<' '<<n->L<<' '<<n->R<<' '<<n->val<<endl;
}

node* Modify_Range(node* now,int r){
    if(r < now->lc->seg_R){
        now->lc = Modify_Range(now->lc,r);
        now->from = r+1;
        now->seg_L = r+1;
        update_L_R(now);
        update_val(now);  
        return now;
    }
    else{
        Delete_Node(now->lc);
        if(r < now->rc->seg_L){
            return now->rc;
        }
        else{
            return Modify_Range(now->rc,r);
        }
    }
}

node* AddRange(node* root,int add_l,int add_r){
    node* newRight = build(add_l,add_r);
    node* newRoot = new node();


    newRoot->seg_L = root->seg_L;
    newRoot->seg_R = newRight->seg_R;

    newRoot->lc = root;
    newRoot->rc = newRight;
    newRoot->from = root->seg_L;
    newRoot->to = newRight->seg_R;

    update_L_R(newRoot);
    
    update_val(newRoot); 

    return newRoot;
}

void DataInit_input(){
//    n = 1000;
//    w = 1000;
//    s = 1;
//    freopen("New_input.txt","r",stdin);
//    freopen("Tree_moving1.txt","w",stdout);

    vector<int> a(n+1);
    prefix.resize(n+1);
    input.resize(n+1);
    for(int i=0;i<n;++i){
        scanf("%lf",&input[i]);
    }
}
void getSum(){
    for(int i = 0;i<n;++i){
        if(i == 0) prefix[i] = input[i];
        else prefix[i] = prefix[i-1] + input[i];
    }
}
void run2(){
    int LL = 0,RR = w-1;

//    printf("AAAAAAAAAAAA\n"); 
    node *root;

    root = build(LL,RR);
    tot = 0;
    while (root->val>0 && tot<k) {
        ans[++tot].val = root->val;
        ans[tot].l = root->val_from;
        ans[tot].r = root->val_to;
//        cout<<root->val_from<<' '<<root->val_to<<' '<<root->val<<endl;
        update(root,root->val_from ,root->val_to);
//           if(root->val>0)  cout<<root->val_from<<' '<<root->val_to<<' '<<root->val<<endl;
    }
//    print_ans();
    
//    int cnt_try = 0;   
    tot_node_cnt_per_window += cnt_node;
    ++cnt_windows;

    while (1 && RR<n-1)
    {
//        ++cnt_try;
//        if(cnt_try==1000) break;
        LL += s;
        RR += s;
        if(RR>n-1) RR=n-1;
        
        reBuild(root);
        root = Modify_Range(root,LL-1);
//        break;
//        Delete_Node(root);
        root = AddRange(root,RR-s+1,RR);   
        //clock_t start = clock();
        
        //clock_t end = clock();
//        printf("BBBBBBBBBBBBBBBB\n");
//        printf("%lf\n",root->val);
        tot = 0;
        tot_node_cnt_per_window += cnt_node;
        ++cnt_windows;
        while (root->val>0 && tot<k) {
            ans[++tot].val = root->val;
            ans[tot].l = root->val_from;
            ans[tot].r = root->val_to;
            update(root,root->val_from ,root->val_to);
//            if(root->val>0)  cout<<root->val_from<<' '<<root->val_to<<' '<<root->val<<endl;
        }
//        print_ans();
        if(RR == n-1) break;
//        printf("\n\n\n\n\n\n");

//        reBuild(root);

/*        while (root->val>0) {
            cout<<root->val_from<<' '<<root->val_to<<' '<<root->val<<endl;
            update(root,root->val_from ,root->val_to);
//            if(root->val>0)  cout<<root->val_from<<' '<<root->val_to<<' '<<root->val<<endl;
        }*/

    
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
        w=n*W*0.01;
        s=w*S*0.01;
        choose_data(n);
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
    freopen("Memory_Streaming_Tournament.txt","a",stdout);
    DataInit_input();
    printf("n=%d s=%d w=%d k=%d    ",n,s,w,k);

    auto start = std::chrono::high_resolution_clock::now();

    getSum();
    run2();

    printf("average_memory_usage_per_window=%.4lf\n",tot_node_cnt_per_window*sizeof(node)/cnt_windows);
    return 0;
}
