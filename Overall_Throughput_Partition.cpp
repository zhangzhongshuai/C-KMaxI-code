#include <bits/stdc++.h>
using namespace std;
const int maxn=1e7+10000;
#define ll long long
ll n,w,s;
int k=0;

double ori[maxn];
double dp[maxn];
double sum[maxn];
int rest_left = -1;

ll cnt_partition = 0;

ll ans_cnt = 0;

struct MapNode{
    double value;
    ll id;

    bool operator < (const MapNode& other) const{
        if(value!=other.value) return value > other.value;
        return id < other.id;
    }
};

struct Map_Compare{
    bool operator()(const MapNode &x, const MapNode &y) const {
        if(x.value != y.value) x.value > y.value;
        return x.id > y.id;
    }
};

struct HeapNode{
    double value;
    int l,r;
};
bool cmp_pos_L(HeapNode a,HeapNode b){return a.value > b.value;}

HeapNode all_ans[maxn];

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

    void print_topk(){
        int cnt_ans=0;
        while(siz!=0){
            all_ans[++cnt_ans] = Pop();
        }
        std::sort(all_ans+1,all_ans+cnt_ans+1,cmp_pos_L);
        for(int i=1;i<=min(cnt_ans,k);++i){
            int x=all_ans[i].value;
            printf("val=%lld\n",x);
//            printf("val=%d l=%d r=%d\n",x,all_ans[i].l,all_ans[i].r);
        }
        printf("cnt_ans=%d\n",cnt_ans);
    }
};
TopKHeap* Max_TopKHead;


class Crest{
public:
    int leftBound;
    int rightBound;
    int domainCrest;
    double value;
    int l;
    int r;
    std::vector<Crest*> merge_crests;
    std::vector<int> merge_place;
    Crest(){
        leftBound = -1;
        rightBound = -1;
        domainCrest = -1;
        l = -1;
        r = -1;
    }


};


class Partition{
public:
    ll id;
    double value;
    double negative_part;
    int l,r;
    int maxPlace;
    TopKHeap* topkheap;
    bool Init_State = false;
    bool deleted=false;
    bool updateHeap = true;
    bool dp_linked = true;

    std::shared_ptr<std::deque<Crest*>> merge_crests;

    Partition(){
        value = 0;
        id = ++cnt_partition;
        maxPlace = -1;
        topkheap =  new TopKHeap();
        merge_crests = std::make_shared<std::deque<Crest*>>();
    }

};
std::shared_ptr<std::map<MapNode,Partition*>> mp_Partition;

std::shared_ptr<std::deque<Crest*>> S_Init;

std::shared_ptr<std::deque<Partition*>> S_MaxPartition;


struct Subsegment{
    int l,r;
    ll val;
}all[maxn];
bool cmp_pos_l(Subsegment a,Subsegment b)
{
    return a.l < b.l;
}

ll max(ll a,ll b){
    return a>b?a:b;
}

int cnt_ans=0;

void MIP(int L,int R,std::shared_ptr<std::deque<Partition*>> S_MaxPartition_tmp,int mod){
    int W=R;
    while(W>=L && ori[W]<=0) --W;
    if(W==L-1){
        if(rest_left==-1) rest_left = L;
        return;
    }
    else{
        if(rest_left!=-1){
            L = rest_left;
        }
        if(W==R) rest_left = -1;
        else rest_left = W+1;
    }

    int MaxEnd = W;
    int minStart = W;
    Partition *nowPartition = new Partition();
    dp[W] = ori[W];
    for(int i=W-1;i>=L;--i){
        if(dp[i+1]<=0 && ori[i]>0){
            nowPartition->r = MaxEnd;
            nowPartition->l = i+1;
            nowPartition->maxPlace = minStart;
            nowPartition->value = dp[minStart];
            nowPartition->negative_part = dp[nowPartition->l] - dp[nowPartition->maxPlace];
            S_MaxPartition_tmp->push_front(nowPartition);
            if(mod==1) (*mp_Partition)[(MapNode){nowPartition->value,nowPartition->id}] = nowPartition;

            nowPartition = new Partition();
            MaxEnd = i;
            minStart = i;
            dp[i] = ori[i];
        }
        else{
            dp[i] = dp[i+1] + ori[i];
            if(dp[i]>dp[minStart]) minStart = i;
        }

    }

    nowPartition->r = MaxEnd;
    nowPartition->l = L;
    nowPartition->maxPlace = minStart;
    nowPartition->value = dp[minStart];
    nowPartition->negative_part = dp[nowPartition->l] - dp[nowPartition->maxPlace];
    S_MaxPartition_tmp->push_front(nowPartition);
    if(mod==1) (*mp_Partition)[(MapNode){nowPartition->value,nowPartition->id}] = nowPartition;

}

int new_Domain_Init(int L,int R,std::shared_ptr<std::deque<Crest*>> crests){
    int r=R;
    int l=L;
    Crest* nowCrest = new Crest();

    while(ori[r]<=0 && r>=l) --r;
    int now_left=r,now_right=r;


    for(int i=r-1;i>=l;){
        if(ori[i]<=0){
            nowCrest->rightBound = now_right;
            nowCrest->leftBound = now_left;
            nowCrest->domainCrest = now_left;
            nowCrest->l = now_left;
            nowCrest->r = now_right;
            nowCrest->value = sum[now_right] - sum[now_left-1];

            crests->push_back(nowCrest);
            nowCrest = new Crest();

            while(ori[i]<=0 && i>=l) --i;
            now_left = i;
            now_right = i;
            --i;
        }
        else{
            now_left = i;
            --i;
        }
    }

    if(now_left==l){
        nowCrest->rightBound = now_right;
        nowCrest->leftBound = now_left;
        nowCrest->domainCrest = now_left;
        nowCrest->l = now_left;
        nowCrest->r = now_right;
        nowCrest->value = sum[now_right] - sum[now_left-1];

        crests->push_back(nowCrest);
    }

    return -1;
}

void Domain_Calculate(std::shared_ptr<std::deque<Crest*>> S_Max,std::shared_ptr<std::deque<Crest*>> S_Init){
    for(int i=S_Init->size()-1;i>=0;--i){
        Crest* right = S_Init->back();
        S_Init->pop_back();

        for(int j=S_Max->size()-1;j>=0;--j){
            Crest* left = S_Max->back();

            double O1 = left->value;
            double O2 = right->value;
            double D1 = abs(sum[right->domainCrest-1] - sum[left->rightBound]);
            if(O1<=D1 && O2>D1){
                right->merge_crests.push_back(left);
                right->l = left->l;
                S_Max->pop_back();
            }
            else{
                if(O1>D1 && O2>D1){
                    right->merge_crests.push_back(left);
                    right->merge_place.push_back(left->domainCrest);
                    right->domainCrest = left->domainCrest;
                    right->value = O1+O2-D1;
                    right->l = left->l;

                    S_Max->pop_back();
                }
                else{
                    break;
                }
            }
        }
        S_Max->push_back(right);

    }
}

void Domain_Calculate_Transfer(std::shared_ptr<std::deque<Crest*>> S_Max,std::shared_ptr<std::deque<Crest*>> S_Init,double extra){

    for(int i=S_Init->size()-1;i>=0;--i){
        Crest* right = S_Init->back();
        S_Init->pop_back();

        for(int j=S_Max->size()-1;j>=0;--j){
            Crest* left = S_Max->back();

            double O1 = left->value;
            double O2 = right->value;
            double D1 = abs(sum[right->domainCrest-1] - sum[left->rightBound]);
            if(O1<=D1 && O2>D1){
                right->merge_crests.push_back(left);
                right->l = left->l;
                S_Max->pop_back();
            }
            else{
                if(O1>D1 && O2>D1){
                    right->merge_crests.push_back(left);
                    right->merge_place.push_back(left->domainCrest);
                    right->domainCrest = left->domainCrest;
                    right->value = O1+O2-D1;
                    right->l = left->l;

                    S_Max->pop_back();
                }
                else{
                    break;
                }
            }
        }
        S_Max->push_back(right);

    }
}

void deletePartitionCrests(std::shared_ptr<std::deque<Crest*>> merge_crests,int r);
void Transfer_One_Partition(Partition* before,std::shared_ptr<std::deque<Crest*>> RightCrests){
    if(!before->Init_State){
        S_Init->clear();
        new_Domain_Init(before->l,before->r,S_Init);
        Domain_Calculate(before->merge_crests,S_Init);
    }
    else{
        if(before->deleted){
            deletePartitionCrests(before->merge_crests,before->l-1);
        }

        Crest* crestNow = new Crest();
        S_Init->clear();
        int l=before->maxPlace,r=before->r;

        new_Domain_Init(before->maxPlace,before->r,S_Init);
        int siz1 = before->merge_crests->size()-1;
        for(int i=siz1;i>=0;--i){
            S_Init->push_back(before->merge_crests->at(i));
        }
        before->merge_crests->clear();
        Domain_Calculate(before->merge_crests,S_Init);
    }
    


    int siz = before->merge_crests->size();
    for(int i=siz-1;i>=0;--i){
       RightCrests->push_front(before->merge_crests->at(i));
    }
    
}

void Transfer_Single_Partition(Partition* p_left,Partition* p_right){
    if(!p_left->Init_State){
        S_Init->clear();
        new_Domain_Init(p_left->l,p_left->maxPlace-1,S_Init);
        Domain_Calculate(p_left->merge_crests,S_Init);
    }
    if(p_left->deleted){
        deletePartitionCrests(p_left->merge_crests,p_left->l-1);
    }

    S_Init->clear();
    int l=p_left->maxPlace,r=p_left->r;
  
    new_Domain_Init(p_left->maxPlace,p_left->r,S_Init);
    Domain_Calculate(p_left->merge_crests,S_Init);

    if(p_right->merge_crests->empty()){
        p_right->merge_crests = p_left->merge_crests;
    }
    else{
        Crest* rightCrest = p_right->merge_crests->front();
        p_right->merge_crests->pop_front();
        S_Init->clear();
        new_Domain_Init(rightCrest->l,rightCrest->r,S_Init);
        Domain_Calculate(p_left->merge_crests,S_Init);
        int cnt=p_left->merge_crests->size()-1;
        for(int i=cnt;i>=0;--i){
            p_right->merge_crests->push_front(p_left->merge_crests->back());
            p_left->merge_crests->pop_back();
        }
    }
}

void addRange(int l,int r){
    std::shared_ptr<std::deque<Partition*>> Tmp_Deque = std::make_shared<std::deque<Partition*>>();
    MIP(l,r,Tmp_Deque,2);

    if(Tmp_Deque->empty()) return;

    Partition* right = Tmp_Deque->front();
    S_Init->clear();
    new_Domain_Init(right->l,right->maxPlace-1,S_Init);
    Domain_Calculate(right->merge_crests,S_Init);
    right->Init_State = true;

    double now_rest_negative = right->negative_part + right->value;
    double postive_part = right->value;
    double negative_part = right->negative_part;
    vector<Partition*> Merged;
    int cnt_merge = 0;
    int merge_place = 0;

    for(int i=S_MaxPartition->size()-1;i>=0;--i){
        Partition* left = S_MaxPartition->back();

        if(now_rest_negative > 0){
            now_rest_negative += left->value + left->negative_part;
            Merged.push_back(left);
            ++cnt_merge;

            if(postive_part>abs(negative_part) && left->value>abs(negative_part)){
                merge_place = cnt_merge;
                postive_part = postive_part + negative_part + left->value;
                negative_part = left->negative_part;
            }
            else{
                negative_part = negative_part + left->negative_part + left->value;
            }
            S_MaxPartition->pop_back();
        }
        else break;
    }

    if(cnt_merge>0){
        Partition* now;
        right->l = Merged.back()->l;
        int i=0;
        for(i=0;i<=cnt_merge-1;++i){
            if(i<merge_place-1){
                mp_Partition->erase((MapNode){Merged.at(i)->value,Merged.at(i)->id});
                delete Merged.at(i);
            }
            else break;
        }

        
        if(merge_place!=0){
            ++i;
            right->maxPlace = Merged.at(merge_place-1)->maxPlace;
            right->value = postive_part;
            right->negative_part = now_rest_negative - postive_part;

            right->merge_crests->clear();

            Partition* beMerged = Merged.at(merge_place-1);
            std::shared_ptr<std::deque<Crest*>> now_Crests = beMerged->merge_crests;
            
            
            right->Init_State = beMerged->Init_State;
            right->deleted = beMerged->deleted;

            if(!now_Crests->empty()){
                for(int j=0;j<=now_Crests->size()-1;++j){
                    right->merge_crests->push_back(now_Crests->at(j));
                }
            }

            mp_Partition->erase((MapNode){Merged.at(merge_place-1)->value,Merged.at(merge_place-1)->id});
            delete Merged.at(merge_place-1);    
        }
        else{
            right->negative_part = now_rest_negative - postive_part;

            for(;i<=cnt_merge-1;++i){
                
                Partition* first = Merged.at(i);
                Transfer_Single_Partition(first,right);
                
                mp_Partition->erase((MapNode){Merged.at(i)->value,Merged.at(i)->id});
                delete Merged.at(i);
            }
            i++;
        }

        if(i<=cnt_merge-1){
            if(!right->Init_State){
                S_Init->clear();
                new_Domain_Init(right->l,right->maxPlace-1,S_Init);
                Domain_Calculate(right->merge_crests,S_Init);
                right->Init_State = true;
            }
        }

        for(;i<=cnt_merge-1;++i){
            Transfer_One_Partition(Merged.at(i),right->merge_crests);
            mp_Partition->erase((MapNode){Merged.at(i)->value,Merged.at(i)->id});
            delete Merged.at(i);
        }
    }


    (*mp_Partition)[(MapNode){right->value,right->id}] = right;      
    S_MaxPartition->push_back(right);
    Tmp_Deque->pop_front();
    int cnt=Tmp_Deque->size()-1;
    for(int i=0;i<=cnt;++i){
        right = Tmp_Deque->front();
        S_MaxPartition->push_back(right);
        (*mp_Partition)[(MapNode){right->value,right->id}] = right;
        Tmp_Deque->pop_front();
    }
}

void deleteCrests(int r,Crest* crest){
    if(r < crest->domainCrest){
        int cnt = crest->merge_crests.size()-1;
        Crest* nowCrest;
        for(int i=cnt;i>=0;--i){
            nowCrest = crest->merge_crests.back();
            if(nowCrest->l > r){
                break;
            }
            else{
                if(nowCrest->r <= r){
                    crest->merge_crests.pop_back();
                    delete nowCrest;
                }
                else{
                    deleteCrests(r,nowCrest);
                }
            }
        }
    }
    else{
        if(r >= crest->leftBound){
            crest->merge_crests.clear();
            crest->merge_place.clear();
            crest->leftBound = r+1;
            crest->domainCrest = r+1;
            crest->l = r+1;
            crest->value = sum[crest->rightBound] - sum[crest->domainCrest-1];
        }
        else{
            int cnt = crest->merge_crests.size()-1;
            Crest* nowCrest;
            for(int i=cnt;i>=0;--i){
                nowCrest = crest->merge_crests.back();
                if(nowCrest->r <=r){
                    crest->merge_crests.pop_back();
                    if(nowCrest->domainCrest == crest->domainCrest){
                        crest->merge_place.pop_back();
                        if(!crest->merge_place.empty()){
                            crest->domainCrest = crest->merge_place.back();
                        }
                        else{
                            crest->domainCrest = crest->leftBound;
                        }
                    }
                    crest->value = sum[crest->rightBound] - sum[crest->domainCrest-1];
                }
                else{
                    bool need_modify = false;
                    if(nowCrest->domainCrest == crest->domainCrest) need_modify = true;
                    deleteCrests(r,nowCrest);
                    int jugdePlace;
                    if(!need_modify) break;
                    crest->merge_place.pop_back();
                    int judge = 0;
                    if(!crest->merge_place.empty()){
                        jugdePlace = crest->merge_place.back();
                    }
                    else{
                        jugdePlace = crest->leftBound;
                    }
                    if(sum[nowCrest->domainCrest-1] < sum[jugdePlace-1]){
                        crest->domainCrest = nowCrest->domainCrest;
                        crest->merge_place.push_back(nowCrest->domainCrest);
                    }
                    else{
                        crest->domainCrest = jugdePlace;
                    }
                    crest->value = sum[crest->rightBound] - sum[crest->domainCrest-1];
                    break;
                }
            }
        }

    }
    crest->l = r+1;
}

void deletePartitionCrests(std::shared_ptr<std::deque<Crest*>> merge_crests,int r){
    Crest* nowCrest;
    if(merge_crests->empty()) return;
    int cnt = merge_crests->size()-1;
    for(int i=0;i<=cnt;++i){
        nowCrest = merge_crests->front();
        if(nowCrest->r <= r){
            merge_crests->pop_front();
            delete nowCrest;
        }
        else{
            break;
        }
    }
    if(merge_crests->empty()){
        return;
    }
    if(nowCrest->l > r){
        return;
    }


    deleteCrests(r,nowCrest);
}

void deleteSinglePartition(Partition* partition,int r){ 
    partition->l = r+1;
    partition->updateHeap = true;

    if(r < partition->maxPlace){
        partition->deleted = true;
        return;
    }
    else{
        dp[partition->r] = ori[partition->r];
        for(int i=partition->r-1;i>=r+1;--i){
            dp[i] = dp[i+1] + ori[i];
        }

        partition->merge_crests->clear();
        int new_minn = partition->r;
        for(int i=partition->r-1;i>r;--i){
            if(dp[i] > dp[new_minn]){
                new_minn = i;
            }
        }
        partition->maxPlace = new_minn;

        double old_value = partition->value;
        partition->value = dp[partition->maxPlace] - dp[partition->r] + ori[partition->r];
        partition->value = sum[partition->r] - sum[partition->maxPlace-1];
        mp_Partition->erase((MapNode){old_value,partition->id});
        (*mp_Partition)[(MapNode){partition->value,partition->id}] = partition;
        partition->Init_State = false;
    }
    
}

void deletePartition(int r){
    for(int i=0;i<S_MaxPartition->size();++i){
        Partition* partition = S_MaxPartition->at(i);
        if(partition->r <= r){
            S_MaxPartition->pop_front();
            mp_Partition->erase((MapNode){partition->value,partition->id});
            delete partition;            
            --i;
        }
        else if(partition->l > r) break;
        else{
            
            partition->l = r+1;
            deleteSinglePartition(partition,r);
        }
    }
}

void judge_random_input(){
    freopen("New_input.txt","r",stdin);
    freopen("SkyCut2.txt","w",stdout);
}


void special_DataInit_input(){
    for(int i=1;i<=n;++i){
        if(i%2==0) ori[i] = i;
        else ori[i] = n-i+1;
    }
}
void Data_Init(){
    int cnt=0;
    for(int i=1;i<=n;++i){
        scanf("%lf",&ori[i]);
        sum[i] = sum[i-1] + ori[i];
    }
}

void Get_ans_stack(Crest* crest,int mod){
    if(mod==1){
        all[++cnt_ans].val = crest->value;
        all[cnt_ans].l = crest->domainCrest;
        all[cnt_ans].r = crest->rightBound;
    }

    Crest* nowCrest;

    for(int i=crest->merge_crests.size()-1;i>=0;--i){
        nowCrest = crest->merge_crests.at(i);
        if(nowCrest->leftBound < crest->domainCrest){
            Get_ans_stack(nowCrest,1);
        }
        else{
            Get_ans_stack(nowCrest,2);
            break;
        }
    }

}

void Get_ans(Partition* partition){
    all[++cnt_ans].val = partition->value;
    all[cnt_ans].l = partition->maxPlace;
    all[cnt_ans].r = partition->r;

    if(partition->merge_crests->empty()){
        return;
    }

    for(auto crest : *(partition->merge_crests)){
        Get_ans_stack(crest,1);
    }
}

void print_ans(){
    cnt_ans = 0;
    for(auto partition : (*S_MaxPartition)){
        Get_ans(partition);
    }
    std::sort(all+1,all+cnt_ans+1,cmp_pos_l);
    for(int i=1;i<=cnt_ans;++i){
        printf("val=%lld\n",all[i].val);
//        printf("val=%lld l=%d r=%d\n",all[i].val,all[i].l,all[i].r);
    }
    printf("cnt_ans=%d\n",cnt_ans);
}

void Get_TopK_crests(Crest* crest,int mod,Partition* partition){
    if(mod==1){
        if(!partition->topkheap->cal_topk((HeapNode){crest->value,crest->domainCrest,crest->rightBound})) return;
    }

    Crest* nowCrest;

    for(int i=crest->merge_crests.size()-1;i>=0;--i){
        nowCrest = crest->merge_crests.at(i);
        if(nowCrest->leftBound < crest->domainCrest){
            Get_TopK_crests(nowCrest,1,partition);
        }
        else{
            Get_TopK_crests(nowCrest,2,partition);
            break;
        }
    }

}

void Get_TopK_partition(Partition* partition){
    partition->topkheap->siz = 0;
   for(auto crest : *(partition->merge_crests)){
        Get_TopK_crests(crest,1,partition);
    }
}

void calc_start(){
    Max_TopKHead->siz = 0;
    auto  it = mp_Partition->begin();
    std::vector<Partition*> topk_Partition;
    int cnt=0;
    for (int i=0; i < k && it != mp_Partition->end(); ++i, ++it) {
        Partition* now = it->second;

        topk_Partition.push_back(now);

        Max_TopKHead->cal_topk((HeapNode){now->value,now->maxPlace,now->r}); 
        ++cnt;       
    }

    for(int i=0;i<=cnt-1;++i){
        Partition* now = topk_Partition.at(i);
        
        bool rebuild_heap = false;

        if(!now->Init_State){
            S_Init->clear();
            new_Domain_Init(now->l,now->maxPlace-1,S_Init);
            Domain_Calculate(now->merge_crests,S_Init);
            now->Init_State = true;
            rebuild_heap = true;
        }

        if(now->deleted){

            deletePartitionCrests(now->merge_crests,now->l-1);
            now->deleted = false;
            rebuild_heap = true;
        }
        
        if(rebuild_heap || now->updateHeap){
            Get_TopK_partition(now);
            now->updateHeap = false;
        }

        if(Max_TopKHead->siz==k   &&   Max_TopKHead->heap[0].value >= now->value) break;

        for(int j=1;j<=now->topkheap->siz;++j){
            Max_TopKHead->cal_topk((HeapNode){now->topkheap->heap[j].value,now->topkheap->heap[j].l,now->topkheap->heap[j].r});
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



int main(int argc, char* argv[]){
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
        freopen("Throughput_Partition.txt","a",stdout);
        printf("n=%d s=%d w=%d k=%d    ",n,s,w,k);
    }
    if(state==2){
        freopen("Overall_Partition.txt","a",stdout);
        printf("precent=%d%%    ",(int)(percent*100.0));
    }
    
    Data_Init();

    auto start = std::chrono::high_resolution_clock::now();
    S_MaxPartition = std::make_shared<std::deque<Partition*>>();
    Max_TopKHead = new TopKHeap();
    mp_Partition = std::make_shared<std::map<MapNode,Partition*>>();

    MIP(1,w,S_MaxPartition,1);

    S_Init = std::make_shared<std::deque<Crest*>>();

    calc_start();

    for(int i=w+1;i<=n;i=i+s){
        deletePartition(i-w+s-1);
        addRange(i,std::min(i+s-1,n));
        calc_start();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    printf("n=%d s=%lf%% w=%lf%% k=%d    ",n,S,W,k);
    std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
    return 0;
}  