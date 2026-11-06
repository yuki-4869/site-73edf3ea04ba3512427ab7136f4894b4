using C=FPS::value_type;
using Vec=vector<C>;
size_t checks=0;
mt19937_64 generator(4869);
void require(bool ok){if(!ok)std::abort();++checks;}
C random_value(){return (ll)(generator()%C::mod());}
FPS random_fps(int count){FPS f(count);for(auto& value:f)value=random_value();return f;}
void trim(FPS& a){while(!a.empty()&&a.back()==C(0))a.pop_back();}
FPS padded(FPS a,int count){a.resize(count);return a;}
FPS naive_mul(const FPS& a,const FPS& b,int limit=INT_MAX){
    if(a.empty()||b.empty()||!limit)return {};
    FPS result(min<int>(limit,a.size()+b.size()-1));
    for(int i=0;i<(int)a.size()&&i<limit;i++)
        for(int j=0;j<(int)b.size()&&i+j<(int)result.size();j++)result[i+j]+=a[i]*b[j];
    return result;
}
FPS naive_inv(const FPS& a,int count){
    FPS result(count);if(!count)return result;
    result[0]=a[0].inv();
    for(int i=1;i<count;i++){
        C sum=0;
        for(int j=1;j<=i&&j<(int)a.size();j++)sum+=a[j]*result[i-j];
        result[i]=-sum*result[0];
    }
    return result;
}
FPS naive_log(const FPS& a,int count){
    FPS result(count);if(count<2)return result;
    FPS derivative(max(0,(int)a.size()-1));
    for(int i=1;i<(int)a.size();i++)derivative[i-1]=a[i]*i;
    auto product=naive_mul(derivative,naive_inv(a,count),count-1);
    for(int i=1;i<count;i++)if(i-1<(int)product.size())result[i]=product[i-1]/i;
    return result;
}
FPS naive_exp(const FPS& a,int count){
    FPS result(count);if(!count)return result;
    result[0]=1;
    for(int i=1;i<count;i++){
        C sum=0;
        for(int j=1;j<=i&&j<(int)a.size();j++)sum+=C(j)*a[j]*result[i-j];
        result[i]=sum/i;
    }
    return result;
}
FPS naive_pow(FPS a,ll exponent,int count){
    FPS result(count);if(!count)return result;
    result[0]=1;a.resize(count);
    while(exponent){
        if(exponent&1)result=padded(naive_mul(result,a,count),count);
        exponent>>=1;
        if(exponent)a=padded(naive_mul(a,a,count),count);
    }
    return result;
}
pair<FPS,FPS> naive_div(FPS a,FPS b){
    trim(a);trim(b);
    if(a.size()<b.size())return {{},a};
    FPS quotient(a.size()-b.size()+1);
    for(int i=(int)quotient.size()-1;i>=0;i--){
        C value=a[i+b.size()-1]/b.back();quotient[i]=value;
        for(int j=0;j<(int)b.size();j++)a[i+j]-=value*b[j];
    }
    trim(a);trim(quotient);return {quotient,a};
}
vector<Vec> binomials(int limit){
    vector<Vec> choose(limit+1);
    for(int n=0;n<=limit;n++){
        choose[n].assign(n+1,1);
        for(int k=1;k<n;k++)choose[n][k]=choose[n-1][k-1]+choose[n-1][k];
    }
    return choose;
}
void core_tests(){
    vector<int> sizes={0,1,2,3,7,16,31,32,33,59,60,61,63,64,65,100,127,128,129,200,255,256,257};
    for(int size:sizes){
        for(int test=0;test<4;test++){
            FPS a=random_fps(size),b=random_fps(test==0?0:1+generator()%150);
            require(a*b==naive_mul(a,b));
            FPS original=a,other=b;
            auto sum=a+b,difference=a-b;
            for(int i=0;i<(int)sum.size();i++){
                C x=i<(int)a.size()?a[i]:C(0),y=i<(int)b.size()?b[i]:C(0);
                require(sum[i]==x+y&&difference[i]==x-y);
            }
            require(a==original&&b==other);
            auto self=a;self+=self;require(self==a*C(2));
            self=a;self-=self;require(self==FPS(a.size()));
            self=a;self*=self;require(self==naive_mul(a,a));
            require(a.pre(size+5)==a);
            for(int shift:{0,1,5,size+1}){
                auto left=a<<shift,right=a>>shift;
                require(left.size()==a.size()+shift);
                for(int i=0;i<(int)left.size();i++)require(left[i]==(i<shift?C(0):a[i-shift]));
                FPS expected;if(shift<size)expected=FPS(a.begin()+shift,a.end());
                require(right==expected);
            }
            require(a.diff().integral()==(a.empty()?FPS{0}:a-FPS{a[0]}));
            if(!size){
                require(a.inv(0).empty()&&a.log(0).empty()&&a.exp(0).empty()&&a.sqrt(0).empty());
                continue;
            }
            a[0]=C(1+generator()%(C::mod()-1));
            require(a.inv(size)==naive_inv(a,size));
            auto identity=padded(naive_mul(a,a.inv(size),size),size);
            require(identity[0]==C(1));
            for(int i=1;i<size;i++)require(identity[i]==C(0));
            a[0]=1;
            require(a.log(size)==naive_log(a,size));
            a[0]=0;
            require(a.exp(size)==naive_exp(a,size));
            for(ll exponent:{0LL,1LL,2LL,5LL})require(a.pow(exponent,size)==naive_pow(a,exponent,size));
            FPS root=random_fps(size);root[0]=C(1+generator()%(C::mod()-1));
            auto square=padded(naive_mul(root,root,size),size);
            auto actual=square.sqrt(size);
            require(actual.size()==(size_t)size);
            C sign=actual[0]==root[0]?C(1):C(-1);
            require(actual==root*sign);
            if(size>2){
                root[0]=0;root[1]=1;square=padded(naive_mul(root,root,size),size);
                actual=square.sqrt(size);
                require(actual.size()==(size_t)size&&padded(naive_mul(actual,actual,size),size)==square);
            }
            require(FPS(size).sqrt(size)==FPS(size));
        }
    }
    for(int test=0;test<300;test++){
        int n=generator()%180,m=1+generator()%100;
        FPS a=random_fps(n),b=random_fps(m);
        if(b.back()==C(0))b.back()=1;
        if(test%3==0){a.resize(n+3);b.resize(m+3);}
        auto expected=naive_div(a,b),actual=a.divmod(b);
        require(expected==actual);
        require(a/b==expected.first&&a%b==expected.second);
        auto left=a;left/=b;require(left==expected.first);
        left=a;left%=b;require(left==expected.second);
        if(!a.empty()){auto clean=a;trim(clean);if(!clean.empty())require(clean.divmod(clean)==pair<FPS,FPS>{{1},{}});}
    }
    auto choose=binomials(160);
    for(int n:{0,1,2,17,31,32,33,63,64,65,100,129}){
        FPS a=random_fps(n);if(n>5)a[n-1]=a[n-2]=0;
        for(C shift:Vec{0,1,2,C(-7),random_value()}){
            FPS expected(n);
            for(int j=0;j<n;j++){
                C power=1;
                for(int i=j;i>=0;i--){expected[i]+=a[j]*choose[j][i]*power;power*=shift;}
            }
            require(a.taylor_shift(shift)==expected);
        }
    }
    for(int test=0;test<300;test++){
        C x=random_value();auto [ok,root]=FPS::mod_sqrt(x*x);
        require(ok&&root*root==x*x&&root.val()<=(-root).val());
    }
    C nonresidue=2;
    while(nonresidue.pow((C::mod()-1)/2)==C(1))nonresidue++;
    require(!FPS::mod_sqrt(nonresidue).first);
    require(FPS{nonresidue}.sqrt(10).empty());
    require(FPS{0,1}.sqrt(10).empty());
    FPS expected(129);expected[0]=1;
    for(int i=1;i<129;i++)expected[i]=expected[i-1]*C(LLONG_MAX-i+1)/i;
    require(FPS{1,1}.pow(LLONG_MAX,129)==expected);
    require(FPS{0,1}.pow(LLONG_MAX,129)==FPS(129));
    FPS long_input(20000);long_input[0]=1;long_input[19999]=9;
    require(long_input.log(8)==FPS(8));
    require(long_input.pow(3,8)==FPS({1,0,0,0,0,0,0,0}));
    require(long_input.sqrt(8)==FPS({1,0,0,0,0,0,0,0}));
    FPS maximal(100,C(C::mod()-1));
    require(maximal*maximal==naive_mul(maximal,maximal));
}
pair<C,C> fibonacci(ll n){
    if(!n)return {0,1};
    auto [a,b]=fibonacci(n/2);
    C first=a*(2*b-a),second=a*a+b*b;
    return n&1?pair<C,C>{second,first+second}:pair<C,C>{first,second};
}
int minimum_order(const Vec& sequence){
    int count=sequence.size();
    for(int order=0;order<=count;order++){
        vector<Vec> rows;
        for(int i=order;i<count;i++){
            Vec row(order+1);row[order]=sequence[i];
            for(int j=0;j<order;j++)row[j]=sequence[i-j-1];
            rows.push_back(move(row));
        }
        int rank=0;
        for(int col=0;col<order;col++){
            int pivot=rank;while(pivot<(int)rows.size()&&rows[pivot][col]==C(0))pivot++;
            if(pivot==(int)rows.size())continue;
            swap(rows[rank],rows[pivot]);C inverse=rows[rank][col].inv();
            for(int j=col;j<=order;j++)rows[rank][j]*=inverse;
            for(int i=0;i<(int)rows.size();i++)if(i!=rank){
                C scale=rows[i][col];for(int j=col;j<=order;j++)rows[i][j]-=scale*rows[rank][j];
            }
            rank++;
        }
        bool valid=true;
        for(const auto& row:rows){bool zero=true;for(int j=0;j<order;j++)zero&=row[j]==C(0);if(zero&&row[order]!=C(0))valid=false;}
        if(valid)return order;
    }
    return count;
}
void rational_tests(){
    require(linear_recurrence({}, {},LLONG_MAX)==C(0));
    require(bostan_mori(FPS{1},FPS{1,-1},LLONG_MAX)==C(1));
    require(linear_recurrence(Vec{0,1},Vec{1,1},LLONG_MAX)==fibonacci(LLONG_MAX).first);
    for(int test=0;test<160;test++){
        FPS numerator=random_fps(generator()%120),denominator=random_fps(1+generator()%70);
        denominator[0]=C(1+generator()%(C::mod()-1));
        Vec sequence(221);
        for(int i=0;i<221;i++){
            C value=i<(int)numerator.size()?numerator[i]:C(0);
            for(int j=1;j<=i&&j<(int)denominator.size();j++)value-=denominator[j]*sequence[i-j];
            sequence[i]=value/denominator[0];
        }
        for(int n:{0,1,2,20,100,220})require(bostan_mori(numerator,denominator,n)==sequence[n]);
    }
    for(int test=0;test<150;test++){
        int order=generator()%13;Vec coefficients(order),sequence(220);
        for(auto& x:coefficients)x=random_value();
        for(int i=0;i<order;i++)sequence[i]=random_value();
        for(int i=order;i<220;i++)for(int j=0;j<order;j++)sequence[i]+=coefficients[j]*sequence[i-j-1];
        Vec initial(sequence.begin(),sequence.begin()+order);
        for(int n:{0,1,50,219})require(linear_recurrence(initial,coefficients,n)==sequence[n]);
        Vec observed(sequence.begin(),sequence.begin()+2*order+5);
        Vec inferred=berlekamp_massey(observed),start(sequence.begin(),sequence.begin()+inferred.size());
        require(linear_recurrence(start,inferred,219)==sequence[219]);
    }
    for(int test=0;test<120;test++){
        Vec data(generator()%10);for(auto& x:data)x=generator()%4;
        auto inferred=berlekamp_massey(data);
        require(inferred.size()==(size_t)minimum_order(data));
        for(int i=inferred.size();i<(int)data.size();i++){
            C value=0;for(int j=0;j<(int)inferred.size();j++)value+=inferred[j]*data[i-j-1];
            require(value==data[i]);
        }
    }
}
void points_and_products_tests(){
    for(int n:{0,1,2,16,31,32,33,50,63,64,65,100,129}){
        Vec points(n);C offset=random_value();for(int i=0;i<n;i++)points[i]=offset+17*i;
        shuffle(points.begin(),points.end(),generator);
        FPSPointSet tree(points);
        FPS product={1};for(C value:points)product=naive_mul(product,FPS{-value,1});
        require(tree.product()==product&&product_linear(points)==product);
        for(int test=0;test<3;test++){
            FPS f=random_fps(test==2?n+113:n);
            Vec expected(n);for(int i=0;i<n;i++)expected[i]=f.eval(points[i]);
            require(tree.eval(f)==expected&&multipoint_eval(f,points)==expected);
            if(test<2){
                FPS restored=f;trim(restored);
                require(tree.interpolate(expected)==restored&&interpolate(points,expected)==restored);
            }
        }
        require(tree.interpolate(Vec(n)).empty());
        Vec duplicate(n);for(int i=0;i<n;i++)duplicate[i]=i%4;
        FPS f=random_fps(145);auto values=multipoint_eval(f,duplicate);
        for(int i=0;i<n;i++)require(values[i]==f.eval(duplicate[i]));
    }
    for(int test=0;test<160;test++){
        vector<FPS> factors(generator()%20);FPS expected={1};
        for(auto& f:factors){f=random_fps(generator()%10);expected=naive_mul(expected,f);}
        trim(expected);auto original=factors;
        require(polynomial_product(factors)==expected&&factors==original);
    }
    require(polynomial_product({FPS{2},FPS{3},FPS{4}})==FPS{24});
    require(polynomial_product({FPS{1},FPS{0}}).empty());
}
void combinatorial_tests(){
    const int limit=100;auto choose=binomials(limit+2);
    vector<Vec> first(limit+1,Vec(limit+1)),second=first;
    first[0][0]=second[0][0]=1;
    for(int n=1;n<=limit;n++)for(int k=1;k<=n;k++){
        first[n][k]=first[n-1][k-1]+C(n-1)*first[n-1][k];
        second[n][k]=second[n-1][k-1]+C(k)*second[n-1][k];
    }
    Vec bernoulli(limit+1);bernoulli[0]=1;
    for(int n=1;n<=limit;n++){
        C sum=0;for(int k=0;k<n;k++)sum+=choose[n+1][k]*bernoulli[k];
        bernoulli[n]=-sum/(n+1);
    }
    for(int n=0;n<=limit;n++){
        auto signed_first=stirling1_signed(n),unsigned_first=stirling1_unsigned(n),actual_second=stirling2_row(n);
        require(signed_first.size()==(size_t)n+1&&unsigned_first.size()==(size_t)n+1&&actual_second.size()==(size_t)n+1);
        for(int k=0;k<=n;k++){
            require(unsigned_first[k]==first[n][k]&&actual_second[k]==second[n][k]);
            require(signed_first[k]==(((n-k)&1)?-first[n][k]:first[n][k]));
        }
        auto actual_bernoulli=bernoulli_numbers(n),actual_bell=bell_numbers(n);
        for(int k=0;k<=n;k++){
            C sum=0;for(int j=0;j<=k;j++)sum+=second[k][j];
            require(actual_bell[k]==sum&&actual_bernoulli[k]==bernoulli[k]);
        }
    }
    for(int n:{0,1,2,32,33,63,64,65,100,257,1024}){
        Vec expected(n+1);expected[0]=1;
        for(int coin=1;coin<=n;coin++)for(int amount=coin;amount<=n;amount++)expected[amount]+=expected[amount-coin];
        require(partition_numbers(n)==expected);
    }
    for(int n=0;n<=80;n++){
        Vec a(n);for(auto& x:a)x=random_value();
        Vec expected(n),inverse_expected(n);
        for(int i=0;i<n;i++)for(int j=0;j<=i;j++){
            expected[i]+=choose[i][j]*a[j];
            inverse_expected[i]+=(((i-j)&1)?-choose[i][j]:choose[i][j])*a[j];
        }
        require(binomial_transform(a)==expected&&inverse_binomial_transform(a)==inverse_expected);
        require(inverse_binomial_transform(expected)==a);
        FPS f=random_fps(n);Vec values(n);for(int i=0;i<n;i++)values[i]=f.eval(i);
        for(C x:Vec{0,1,C(n),C(-1),C(1000000000000LL)})require(lagrange_consecutive(values,x)==f.eval(x));
    }
}
void invalid_case(string action){
    if(action=="negative_prefix")FPS{1}.pre(-1);
    if(action=="negative_shift")FPS{1}<<-1;
    if(action=="inverse_zero")FPS{0}.inv(3);
    if(action=="log_constant")FPS{2}.log(3);
    if(action=="exp_constant")FPS{1}.exp(3);
    if(action=="negative_power")FPS{1}.pow(-1,3);
    if(action=="divide_zero")FPS{1}/C(0);
    if(action=="polynomial_zero")FPS{1}.divmod(FPS{});
    if(action=="duplicate_points")FPSPointSet(Vec{1,1}).interpolate(Vec{2,2});
    if(action=="point_count")FPSPointSet(Vec{1,2}).interpolate(Vec{3});
    if(action=="recurrence_count")linear_recurrence(Vec{1},Vec{},2);
    if(action=="negative_partition")partition_numbers(-1);
}
int main(int argc,char** argv){
    if(argc>1){invalid_case(argv[1]);return 77;}
    core_tests();rational_tests();points_and_products_tests();combinatorial_tests();
    cout<<"mod="<<C::mod()<<" checks="<<checks<<" passed\n";
}
