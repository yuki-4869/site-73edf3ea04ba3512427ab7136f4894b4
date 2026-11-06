using C=FPS::value_type;
using Vec=vector<C>;
void require(bool ok){if(!ok)std::abort();}
int main(){
    mt19937_64 random(4869);
    int n=C::mod()==998244353?65539:16387;
    FPS f(n);f[0]=1;for(int i=1;i<n;i++)f[i]=(ll)(random()%C::mod());
    auto inverse=f.inv(n);
    auto identity=(f*inverse).pre(n);
    require(identity.size()==(size_t)n&&identity[0]==C(1));
    for(int i=1;i<n;i++)require(identity[i]==C(0));
    for(int test=0;test<12;test++){
        int index=random()%n;C value=0;
        for(int j=0;j<=index;j++)value+=f[j]*inverse[index-j];
        require(value==C(index==0));
    }
    auto square=(f*f).pre(n);require(square.sqrt(n)==f);
    auto logarithm=f.log(n);require(logarithm.exp(n)==f);
    f[0]=0;auto exponential=f.exp(n);
    require((f.diff()*exponential).pre(n-1)==exponential.diff());
    for(int test=0;test<12;test++){
        int index=1+random()%(n-1);C value=0;
        for(int j=1;j<=index;j++)value+=C(j)*f[j]*exponential[index-j];
        require(value==C(index)*exponential[index]);
    }
    f.resize(2049);C shift=7654321;auto shifted=f.taylor_shift(shift);
    for(int test=0;test<10;test++){C x=(ll)random();require(shifted.eval(x)==f.eval(x+shift));}

    Vec points(1001);for(int i=0;i<(int)points.size();i++)points[i]=i*31+7;
    FPSPointSet tree(points);
    f.resize(points.size());
    Vec expected(points.size());for(int i=0;i<(int)points.size();i++)expected[i]=f.eval(points[i]);
    require(tree.eval(f)==expected);
    require(tree.interpolate(expected)==f);
    f[3]+=9;for(int i=0;i<(int)points.size();i++)expected[i]=f.eval(points[i]);
    require(tree.interpolate(expected)==f);
    require(tree.product()==product_linear(points));

    int degree=8193;
    Vec roots(degree);iota(roots.begin(),roots.end(),0);
    require(stirling1_signed(degree)==product_linear(roots));
    int count=10000;
    Vec partitions(count+1);partitions[0]=1;
    for(int coin=1;coin<=count;coin++)for(int amount=coin;amount<=count;amount++)partitions[amount]+=partitions[amount-coin];
    require(partition_numbers(count)==partitions);
    cout<<"mod="<<C::mod()<<" large_precision="<<n<<" points=1001 stirling=8193 partitions=10000 passed\n";
}
