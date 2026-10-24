void dijkstra_test_require(bool condition){
    if(!condition) abort();
}

void dijkstra_test_compare(const graph& g,const Graph& wg,const vl& starts){
    ll n=g.size();
    vector<vl> a(n,vl(n,inf)),b(n,vl(n,inf));
    for(ll i=0;i<n;i++){
        a[i][i]=b[i][i]=0;
        for(ll to:g[i]) a[i][to]=min(a[i][to],1LL);
        for(auto [to,w]:wg[i]) b[i][to]=min(b[i][to],w);
    }
    for(auto* matrix:{&a,&b}) for(ll k=0;k<n;k++) for(ll i=0;i<n;i++) for(ll j=0;j<n;j++){
        LL sum=(LL)(*matrix)[i][k]+(*matrix)[k][j];
        if(sum<(*matrix)[i][j]) (*matrix)[i][j]=(ll)sum;
    }
    vl expected(n,inf),weighted(n,inf);
    for(ll s:starts) for(ll i=0;i<n;i++){
        expected[i]=min(expected[i],a[s][i]);
        weighted[i]=min(weighted[i],b[s][i]);
    }
    const graph saved_g=g;
    const Graph saved_wg=wg;
    dijkstra_test_require(dijkstra(g,starts)==expected);
    dijkstra_test_require(dijkstra(wg,starts)==weighted);
    dijkstra_test_require(g==saved_g && wg==saved_wg);
    if(starts.size()==1){
        ll s=starts[0];
        dijkstra_test_require(dijkstra(g,s)==expected);
        dijkstra_test_require(dijkstra(wg,s)==weighted);
        dijkstra_test_require(dijkstra(g,{s})==expected);
        dijkstra_test_require(dijkstra(wg,{s})==weighted);
    }
    if(starts.size()==2){
        dijkstra_test_require(dijkstra(g,{starts[0],starts[1]})==expected);
        dijkstra_test_require(dijkstra(wg,{starts[0],starts[1]})==weighted);
    }
    if(starts.empty()){
        dijkstra_test_require(dijkstra(g,{})==expected);
        dijkstra_test_require(dijkstra(wg,{})==weighted);
    }
}

void dijkstra_test_all(){
    dijkstra_test_require(Pow2.size()==64);
    for(size_t i=0;i<64;i++) dijkstra_test_require(Pow2[i]==(1ULL<<i));
    dijkstra_test_require((LL)Pow2[63]==((LL)1<<63));
    static_assert(is_same_v<decltype(dijkstra(declval<const graph&>(),0LL)),vl>);
    static_assert(is_same_v<decltype(dijkstra(declval<const Graph&>(),declval<const vl&>())),vl>);
    dijkstra_test_compare(graph{},Graph{},{});
    dijkstra_test_compare(graph(1),Graph(1),{});
    dijkstra_test_compare(graph(1),Graph(1),{0});
    dijkstra_test_compare(graph(1),Graph(1),{0,0,0});
    graph g={{0,1,1},{0,1,2},{3},{2},{}};
    Graph wg={{{0,0},{1,10},{1,1}},{{0,0},{1,0},{2,0}},{{3,0}},{{2,0}},{}};
    for(vl starts:{vl{},vl{0},vl{1},vl{0,2},vl{0,0},vl{4},vl{0,1,2,3,4}})
        dijkstra_test_compare(g,wg,starts);
    graph overflow_g={{1,2,3},{2,3},{3},{4},{}};
    Graph overflow_wg={{{1,inf-2},{2,inf},{3,inf-1}},{{2,1},{3,10}},{{3,0}},{{4,1}},{}};
    dijkstra_test_compare(overflow_g,overflow_wg,{0});
    dijkstra_test_require(dijkstra(overflow_wg,0)==vl({0,inf-2,inf-1,inf-1,inf}));
    dijkstra_test_require(dijkstra(overflow_wg,{0,4})==vl({0,inf-2,inf-1,inf-1,0}));
    mt19937_64 random(20261025);
    for(int test=0;test<3000;test++){
        ll n=1+random()%10;
        graph rg(n);
        Graph rwg(n);
        bool undirected=random()%2;
        int edges=random()%101;
        for(int edge=0;edge<edges;edge++){
            ll v=random()%n,to=random()%n;
            ll w=random()%31;
            if(test%4==0){
                switch(random()%5){
                    case 0:w=inf;break;
                    case 1:w=inf-1-random()%100;break;
                    case 2:w=inf/2+random()%100;break;
                }
            }
            rg[v].push_back(to);
            rwg[v].push_back({to,w});
            if(undirected){rg[to].push_back(v);rwg[to].push_back({v,w});}
        }
        vl starts;
        ll count=random()%(2*n+1);
        for(ll i=0;i<count;i++) starts.push_back(random()%n);
        dijkstra_test_compare(rg,rwg,starts);
        dijkstra_test_compare(rg,rwg,{(ll)(random()%n)});
    }
    const ll n=200000;
    graph chain(n);
    Graph weighted_chain(n);
    for(ll i=0;i+1<n;i++){
        chain[i].push_back(i+1);
        weighted_chain[i].push_back({i+1,1000000000LL});
    }
    auto unweighted=dijkstra(chain,0);
    auto weighted=dijkstra(weighted_chain,0);
    for(ll i=0;i<n;i++){
        dijkstra_test_require(unweighted[i]==i);
        dijkstra_test_require(weighted[i]==i*1000000000LL);
    }
    vl repeated(n,0);
    dijkstra_test_require(dijkstra(chain,repeated)==unweighted);
    dijkstra_test_require(dijkstra(weighted_chain,repeated)==weighted);
    Graph parallel(2);
    for(ll w=n;w>=0;w--) parallel[0].push_back({1,w});
    dijkstra_test_require(dijkstra(parallel,0)==vl({0,0}));
    cout<<"Dijkstra passed: 3000 randomized graphs, boundary cases, 200000-vertex chain and parallel edges; all 64 Pow2 values passed.\n";
}
