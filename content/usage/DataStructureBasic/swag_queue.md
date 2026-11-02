# SWAG（集約付きキュー）

[説明書の目次](../index.md) · [コード](../../../DataStructureBasic/swag_queue.cpp)

FIFOのキューに対し、末尾への追加・先頭の削除・全要素の集計を行います。
追加と削除は償却O(1)、集計は最悪O(1)です。区間最小値・最大値・GCD・行列積・関数合成などを、尺取り法やスライディングウィンドウで管理する場合に使えます。

SWAGはSliding Window Aggregationの略です。この実装は2本のスタックで集約値を保持します。演算に結合則と単位元があれば使え、逆元や可換性は要求しません。
型エイリアスと貼り付け方は [共通設定](../common.md) を参照してください。基本テンプレートの標準ヘッダーだけで動作し、追加includeやACLへの依存はありません。

## 型・前提

```cpp
template<class S, S(*op)(S,S), S(*e)()>
class SwagQueue;
```

Sは要素と集約値の型です。`op(a,b)` はaの右にbを連結する演算、`e()` は単位元です。例えば区間和では `op(a,b)=a+b`、`e()=0` とします。

- `op(op(a,b),c) == op(a,op(b,c))`、`op(e(),a) == op(a,e()) == a` を満たす必要があります。
- 順序を保って集約するため、可換性は不要です。逆元・冪等性も使いません。
- Sはコピー構築・コピー代入できる型とします。デフォルトコンストラクタは不要です。
- opとeの結果は、呼び出す順番や外部状態によって変わらないことが必要です。
- 途中の計算も整数型の範囲内に収めてください。浮動小数点では丸め誤差で結合則が崩れる場合があります。
- 現在の要素数をN、構築またはコピー後の履歴も含めた過去最大の保持要素数をPとします。必要なメモリを確保できることが前提です。
- 以下の計算量はSのコピー・破棄、op、eがO(1)の場合です。文字列連結など、値の長さに応じて演算コストが増える場合は別に数えてください。

先頭を位置0とする並び順で扱います。任意の位置へアクセス・挿入・更新するAPIはありません。

## コンストラクタ

```cpp
SwagQueue<S,op,e> q;
```

空キューを構築します。構築時にopやeは呼びません。
通常の関数またはキャプチャなしのconstexprラムダをテンプレート引数として渡します。

**計算量**：時間・領域ともO(1)。

## SwagQueue::size

```cpp
size_t size() const;
```

現在の要素数Nを返します。O(1)です。返り値は符号なしのsize_tです。

## SwagQueue::empty

```cpp
bool empty() const;
```

キューが空ならtrue、それ以外ならfalseを返します。O(1)です。

## SwagQueue::push

```cpp
void push(const S& value);
```

末尾へvalueを1個追加します。返り値はありません。valueの内容をコピーして保持するため、後から呼び出し元の値を書き換えてもキューには影響しません。

**計算量**：償却O(1)。vectorの再確保がある1回は最悪O(N+1)です。

## SwagQueue::pop

```cpp
void pop();
```

先頭の1要素を削除します。返り値はありません。呼び出し前に `!empty()` が必要です。
削除する値が必要なら、先にfrontで取得してください。

**計算量**：償却O(1)。先頭側スタックが空の場合、末尾側の全要素を移し替えるため、1回では最悪O(N)です。
この移し替えは各要素につき高々1回なので、空から始めた一連のpush・popは合計操作回数に比例する時間で処理できます。
コピー済みのキューから始める場合は、複製された初期要素数の分も含めて数えます。

## SwagQueue::front

```cpp
S front() const;
```

先頭要素の値をコピーして返します。呼び出し前に `!empty()` が必要です。
返された値を書き換えても内部の値は変わりません。

**計算量**：最悪O(1)。スタックの移し替えは行わず、constのキューからも呼べます。

## SwagQueue::all_prod

```cpp
S all_prod() const;
```

現在の要素を先頭から末尾の順にopで集約した値を返します。
要素列を $a_0,a_1,\ldots,a_{N-1}$ とすると、$op(\cdots op(op(e(),a_0),a_1),\ldots,a_{N-1})$ に対応します。
空ならe()、1要素ならその値を返します。

**計算量**：最悪O(1)。opの呼び出しは高々1回です。
結果をコピーして返すため、返り値の変更は内部の集約値へ影響しません。キューの内容や配置も変更しません。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
constexpr auto sum_op=[](ll a,ll b){return a+b;};
constexpr auto sum_e=[](){return 0LL;};
SwagQueue<ll,sum_op,sum_e> q;
assert(q.empty() && q.size()==0 && q.all_prod()==0);
q.push(3);q.push(1);q.push(4);
assert(q.size()==3 && q.front()==3 && q.all_prod()==8);
q.pop();
q.push(5);
const auto& read=q;
assert(read.front()==1 && read.all_prod()==10);
q.pop();q.pop();q.pop();
assert(q.empty() && q.all_prod()==0);

// 長さ3の窓の最小値
constexpr auto min_op=[](ll a,ll b){return min(a,b);};
constexpr auto min_e=[](){return numeric_limits<ll>::max();};
SwagQueue<ll,min_op,min_e> window;
vl values={3,1,4,1,5},answer;
for(ll x:values){
    window.push(x);
    if(window.size()>3)window.pop();
    if(window.size()==3)answer.push_back(window.all_prod());
}
assert((answer==vl{1,1,1}));

// fの後にgを適用する関数合成
using F=pair<mint,mint>;
constexpr auto compose=[](F f,F g)->F{
    return {g.first*f.first,g.first*f.second+g.second};
};
constexpr auto identity=[]()->F{return {1,0};};
SwagQueue<F,compose,identity> functions;
functions.push({2,1});functions.push({3,4});functions.push({5,6});
auto [a,b]=functions.all_prod();
assert(a*mint(7)+b==mint(251));
functions.pop();
auto [c,d]=functions.all_prod();
assert(c*mint(7)+d==mint(131));
```

関数合成の例では `2x+1`、`3x+4`、`5x+6` を追加順に適用します。opの引数順を逆にすると別の関数になるため、先頭からの適用順を明示しています。

## 仕組みと償却計算量

末尾側のスタックには、底から上に追加順で要素を並べ、`op(これまでの集約値,追加値)` を保存します。
先頭側が空になってpopを行うとき、末尾側を上から取り出して先頭側へ積みます。
このとき `op(移した値,先頭側の集約値)` とすることで、取り出す順に積を保持します。

先頭側の頂上がキューの先頭です。2つのスタックが両方非空なら、`op(先頭側の集約値,末尾側の集約値)` がキュー全体の集約値になります。
先頭側が空のときにfrontを呼ぶ場合は、末尾側の底の要素を直接返すので、移し替えは不要です。

各要素は末尾側へ1回入り、先頭側へ高々1回移り、高々1回削除されます。vectorの拡張を含め、空から始めた一連の操作は償却O(1)で処理できます。
1回のpopの最悪時間がO(1)になる実装ではありません。

## コピー・ムーブ・メモリ

コピーは現在の要素と集約値を独立に複製します。一方のキューを変更しても他方は変わりません。
コピー構築はO(N+1)時間・領域で、ムーブ構築はO(1)です。代入には、これらに加えて代入先の以前の要素を解放するコストがかかります。
ムーブ元の内容には依存せず、再使用する場合は空キューなどを代入してください。

生存しているN要素について、値と集約値を各1個保持します。vectorの容量はpopで縮まないため、確保領域は現在のNではなく過去最大Pに対してO(P+1)です。
キューを破棄するまで、確保済み容量が残る場合があります。

## 入力検査・確認結果

空のpopとfrontは、NDEBUGでもメッセージなしで停止します。型の演算則や整数のオーバーフローを自動で検査するものではありません。

専用テスト `tests/swag_queue_test.cpp` で、操作列の全探索とランダム操作をstd::dequeの直接集約に照合します。
区間和・最小値・非可換な行列積と文字列連結、空・1要素、移し替え前後、コピー・ムーブ、デフォルト構築できない値の型を確認します。
大きなキューでは集約演算の呼び出し回数も検証し、C++23・NDEBUG・AddressSanitizer／UBSanで確認します。

## 参考

[Sliding-Window Aggregation Algorithms（著者によるチュートリアル）](https://hirzels.com/martin/papers/debs17-tutorial.pdf)：集約付きスタックとTwo-StacksによるFIFO窓の集計を扱っています。
