struct TemplateOutputBuffer: stringbuf{int flushes=0;int sync() override{++flushes;return 0;}};
void template_print_test_all(){
    TemplateOutputBuffer buffer;
    auto* previous=cout.rdbuf(&buffer);
    auto flags=cout.flags();auto precision=cout.precision();
    cout<<defaultfloat<<setprecision(6);
    p(1.25L);p2(1,2);p3(1,2,3);p4(1,2,3,4);
    pld(1.25L);pld2(1,2.5);pld3(-1,0,3);pld4(1,2,3,4);
    p(1.25L);p(mint(7));
    int calls=0;pld2((++calls,1),(++calls,2));
    cout_ver(vl{3,1,4});cout_ver(vector<string>{"a","b"});cout_ver(vl{});
    if(calls!=2 || buffer.flushes!=0) abort();
    pe("?");pe2("?",2);pe3("?",2,3);pe4("?",2,3,4);
    if(buffer.flushes!=4) abort();
    string expected="1.25\n1 2\n1 2 3\n1 2 3 4\n"
      "1.250000000000\n1.000000000000 2.500000000000\n"
      "-1.000000000000 0.000000000000 3.000000000000\n"
      "1.000000000000 2.000000000000 3.000000000000 4.000000000000\n"
      "1.25\n7\n1.000000000000 2.000000000000\n3\n1\n4\na\nb\n?\n? 2\n? 2 3\n? 2 3 4\n";
    if(buffer.str()!=expected) abort();
    buffer.str("");cout<<scientific<<setprecision(4);
    auto scientific_flags=cout.flags();
    pld2(1.25,2);
    if(cout.flags()!=scientific_flags || cout.precision()!=4) abort();
    p(1.25L);
    if(buffer.str()!="1.250000000000 2.000000000000\n1.2500e+00\n") abort();
    cout.flags(flags);cout.precision(precision);cout.rdbuf(previous);
    cout<<"Output tests passed: p, pld1-4, pe1-4 flushes, cout_ver, and restored formatting.\n";
}
