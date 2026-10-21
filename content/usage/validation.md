# C++23での確認結果

## 2026-10-17の全体整形後の検証

`code`・`codet`・`codecf` の3テンプレートは、先頭のGitHubコードページへのリンク1行だけをコメントとして残しています。それ以外の62ファイルの末尾は「細かい注意点 → 使える関数一覧 → 基本の関数」の順です。実装本体から余分なコメントとライブラリ側のincludeを除去しました。
`code`・`codet`・`codecf`との貼り付け、基本例とサイトの使用例で252件のコンパイル・実行を確認しています。codecfには共通設定に記したACLヘッダーを補います。
RangeSetの524,288操作、座標圧縮の131,072ステップ、セグ木の163,840グループステップを含む回帰テスト、およびAND順列の独立した二部マッチングとの比較も成功しています。
前提違反のテストは子プロセスで実行し、無言で停止することを確認します。座標圧縮・セグ木はNDEBUGでも検証しています。テストはサイト用リポジトリのtestsに保存しています。

以下は従来の改訂・検証記録です。例外に関する記述は当時の結果であり、現在の検出時の動作は「メッセージなしで停止」に統一されています。


[使い方の目次](index.md) · [優先して確認・修正する項目](../review-notes.md#優先して確認修正する項目)

確認日: 2026-10-15（日本時間）

## RangeSet改訂後の追加確認

[改訂版コード](https://github.com/yuki-4869/Algorithm/blob/56ecc78753f54a85b653e23783dcab7ea0c48eb6/DataStructureBasic/range_set.cpp) と [専用テスト](../../tests/range_set_test.cpp) を確認しました。
RangeSetの新しい掲載例と旧不具合の再現ケースは、ソースを書き換えずC++23で成功しています。
その後の座標圧縮・遅延セグ木・Beats拡張の追加改訂と確認は、下の節を参照してください。

| 確認内容 | 結果 |
| --- | --- |
| 新しい掲載例と基本テンプレートとの組み合わせ | コンパイル・実行成功 |
| 専用テスト（ソースの標準ヘッダだけで使用） | コンパイル・実行成功 |
| 整数集合とのランダム比較 | int / long long各262,144操作、合計524,288操作で一致 |
| 座標・個数の型の端、空区間、コピー・ムーブ | 成功 |
| AddressSanitizer・UBSan・デバッグイテレータ | エラーなし。実行環境の制限でLeakSanitizerは無効 |

```sh
g++ -std=c++23 -O2 tests/range_set_test.cpp -o /tmp/range_set_test
/tmp/range_set_test
python3 tests/check_examples.py \
  --only DataStructureBasic/range_set.cpp \
  --acl /path/to/ac-library --output /tmp/range_set_examples
```

以下の全ファイル検証のうち、RangeSet・座標圧縮・遅延セグ木・Beats拡張の結果は改訂版で更新しています。

## 座標圧縮改訂後の追加確認

[改訂版コード](https://github.com/yuki-4869/Algorithm/blob/0eacd5e38b1bf98e3870a77821d54cee82f011e0/DataStructureBasic/coordinate_compression.cpp) と [専用テスト](../../tests/coordinate_compression_test.cpp) を確認しました。
新しい掲載例は、宣言順の書き換えなどをせず、そのままC++23で成功しています。座標圧縮の改訂時点では、他の64ファイルのソースを変更していません。その後の変更は次の節に記録しています。

| 確認内容 | 結果 |
| --- | --- |
| 専用テストと基本テンプレートを併用する掲載例 | コンパイル・実行成功 |
| 独立な集合とのランダム比較 | 昇順long long / 降順int各65,536ステップ、計131,072ステップで一致 |
| 空表・未構築・未登録値・範囲外番号・自己参照・コピー・ムーブ | 成功 |
| 型の最小値・最大値、文字列・pair・tuple・bool、独自の比較器 | 成功 |
| NDEBUGを有効にした検証 | 成功。未構築・境界の検出も有効 |
| AddressSanitizer・UBSan・デバッグイテレータ | エラーなし。実行環境の制限でLeakSanitizerは無効 |

```sh
g++ -std=c++23 -O2 tests/coordinate_compression_test.cpp -o /tmp/coordinate_zip_test
/tmp/coordinate_zip_test
python3 tests/check_examples.py \
  --only DataStructureBasic/coordinate_compression.cpp \
  --acl /path/to/ac-library --output /tmp/coordinate_zip_examples
```

## 遅延セグ木・Beats拡張改訂後の追加確認

[遅延セグ木の改訂コード](https://github.com/yuki-4869/Algorithm/blob/f9177dec185cbc540c81fa7c94c944c1e87348ac/DataStructureSegment/lazy_segtree_wrappers.cpp)、[Beats拡張の改訂コード](https://github.com/yuki-4869/Algorithm/blob/f9177dec185cbc540c81fa7c94c944c1e87348ac/DataStructureSegment/segment_tree_beats_extra.cpp) と [専用テスト](../../tests/segment_wrappers_test.cpp) を確認しました。
この改訂で変更した配布ソースはこの2ファイルだけです。他の63ファイルの結果は直前の検証結果を引き継いでいます。

| 確認内容 | 結果 |
| --- | --- |
| 改訂前・改訂後それぞれの掲載例 | 2 / 2件ともコンパイル・実行成功。ソース修正による対処は不要 |
| 遅延セグ木17種類・Beats拡張4種類 | 既定構築・要素数・配列・コピー・ムーブ・空状態・入力検査に成功 |
| 独立な配列とのランダム比較 | 32 seed × 1,024ステップ × 5群、計163,840グループステップで一致 |
| 個別確認 | 負係数の一次変換・別mod・転倒数・等差数列・境界探索・商の一括更新・整数平方根・bit62に成功 |
| 最適化・警告・NDEBUG | `-O2 -DNDEBUG -Wall -Wextra -Wconversion -Wshadow -Werror`で成功 |
| AddressSanitizer・UBSan・デバッグイテレータ | エラーなし。実行環境の制限でLeakSanitizerは無効 |

各グループステップでは複数の木を同時に照合しています。Beatsの非線形更新は単発最悪O(N)で、任意の混在操作列への一律の対数償却評価はしていません。値・中間結果・和のlong longへの収まりは利用側の制約です。

```sh
g++ -std=c++23 -O2 -DNDEBUG -Wall -Wextra -Wconversion -Wshadow -Werror \
  -I/path/to/ac-library tests/segment_wrappers_test.cpp -o /tmp/segment_wrappers_test
/tmp/segment_wrappers_test
python3 tests/check_examples.py \
  --only DataStructureSegment/lazy_segtree_wrappers.cpp \
  --only DataStructureSegment/segment_tree_beats_extra.cpp \
  --acl /path/to/ac-library --output /tmp/segment_wrappers_examples
```

## 結果

| 確認内容 | 結果 |
| --- | --- |
| 各ページの準備・対処を行った掲載例 | **65 / 65件でコンパイル・実行成功** |
| 同じ依存定義で元ファイルの不備を直さずにコンパイル | **61 / 65件で成功、4件でエラー** |
| 掲載例以外の個別確認 | RangeSet・座標圧縮・遅延セグ木・Beats拡張の旧不具合は修正・再確認済み。負の完全平方数の約数列挙には不具合が残る |

65件はC++コード64ファイルと素数データ1ファイルです。
このライブラリは貼り付け用スニペット集なので、元ファイルの確認でも型エイリアス・マクロ・ACLなどの依存定義とmainを用意しています。
追記用main、関数内スニペット、穴埋め用の演算は完成例として確認しています。成功件数は各ファイルが単独でコンパイルできることや、全APIの成功を表すものではありません。

## 環境・再現方法

| 項目 | 使用した環境 |
| --- | --- |
| コンパイラ | GCC 13.3.0（Ubuntu 13.3.0-6ubuntu2~24.04） |
| 言語・オプション | `-std=c++23 -O0`（assertを有効にして実行） |
| ACL | [864245a00b00dd008d1abfdc239618fdb7d139da](https://github.com/atcoder/ac-library/tree/864245a00b00dd008d1abfdc239618fdb7d139da) |
| 実行環境 | Linux、64bit |

[検証スクリプト](../../tests/check_examples.py)は、説明ページの「使用例」を読み取り、一時フォルダ内でコードを組み立てます。配布するC++ファイルは書き換えません。

```sh
python3 tests/check_examples.py \
  --acl /path/to/ac-library \
  --output /tmp/algorithm-cpp23-checks
```

コンパイルログと結果のJSONは指定した出力先に保存します。
スクリプトの終了コード0は選択した掲載例（指定なしなら65件）とRangeSet・座標圧縮・遅延セグ木・Beats拡張の回帰ケースの成功を表します。元コードのエラーと個別の不具合確認はJSONの `raw` と `targeted` を参照してください。

## 元ファイルのままではコンパイルできなかったもの

| 対象 | 原因 | 掲載例で行った対処 |
| --- | --- | --- |
| [動的連結性（オンライン）](DataStructureAdvanced/dynamic_connectivity.md) | 全角空白が識別子として解釈される | コメント外の全角空白を半角に置換 |
| [Cartesian Tree](Tree/cartesian_tree.md) | グローバル使用例のvが未宣言 | クラス定義を使い、呼び出し例をmainへ移動 |
| [全方位木DP](Tree/rerooting.md) | 未記入のDP identity()がreturn;のまま | 汎用クラスだけを使用するか、問題固有のDP例を完成 |
| [動的連結性（オフライン）](Graph/offline_dynamic_connectivity.md) | 基本テンプレートのpマクロとp(n)が衝突 | 定義の前で#undef pを行う、またはマクロ名を変更 |

`rerooting.cpp` の問題固有の型・演算は記入用の例です。汎用の `Rerooting` 本体を使う掲載例は成功しています。

## 旧版の再現結果と改訂後の状態

| 対象 | 確認した操作 | 旧版の結果 | 現行版の結果 |
| --- | --- | --- | --- |
| [RangeSet::erase](DataStructureBasic/range_set.md) | insert(1,10) → erase(3,7) | setの要素への代入でコンパイルエラー | 成功。削除数4、総被覆数5 |
| [RangeSet::covered / mex](DataStructureBasic/range_set.md) | insert(5,10) → insert(1,5) → covered(1,10), mex(1) | false, 5 | true, 10 |
| [RangeSet::erase](DataStructureBasic/range_set.md) | 旧版の代入エラーだけを除去して部分削除を確認 | 削除数7、総被覆数2 | 改訂版は検証用修正不要。削除数4、総被覆数5 |
| [座標圧縮](DataStructureBasic/coordinate_compression.md) | 既定のusing宣言と構築状態 | 旧版は宣言順でコンパイルエラー。NDEBUGでは未構築の検査も消える | 宣言順を修正。状態・境界検査をNDEBUGでも実施 |
| [永続セグメント木](DataStructureSegment/persistent_segment_tree.md) | NODES=1の上限超過更新、N=LLONG_MAXの末尾更新 | AddressSanitizerで範囲外書き込み、UBSanで中点の整数オーバーフローを再現 | 容量超過は書き込み前に停止。中点計算を修正し、巨大な添字での更新・取得・境界探索が成功 |
| [約数列挙](MathNumberTheory/divisors.md) | get_divisors(-4) | -4 -2 -2 -1 1 2 2 4 | 未修正。期待値は -4 -2 -1 1 2 4 |

旧RangeSetの結果は [改訂前の版](https://github.com/yuki-4869/Algorithm/blob/335d61be5c0f520c94a7bc0a81b3257b52dae4f4/DataStructureBasic/range_set.cpp) に対する記録です。

## ファイル別の確認結果

| 説明ページ | 掲載例（C++23） | 元コードのコンパイル・準備 |
| --- | --- | --- |
| [約数列挙](MathNumberTheory/divisors.md) | コンパイル・実行成功 | 成功 |
| [mod累乗・逆元](MathNumberTheory/mod_pow_inverse.md) | コンパイル・実行成功 | 成功 |
| [10^6 以下の素数表](MathNumberTheory/primes_upto_1000000.md) | コンパイル・実行成功 | 成功（全78,498個を初期化子として使用） |
| [回文半径](String/palindrome_radius.md) | コンパイル・実行成功 | 成功（Sを用意し関数内に配置） |
| [動的連結性（オンライン）](DataStructureAdvanced/dynamic_connectivity.md) | コンパイル・実行成功 | エラー（対処は上表） |
| [動的連結性（重み付き・オフライン）](Graph/offline_dynamic_connectivity_weighted.md) | コンパイル・実行成功 | 成功 |
| [区間集合（改訂版）](DataStructureBasic/range_set.md) | コンパイル・実行成功 | 成功。回帰・ランダム・型の端も追加確認 |
| [Cartesian Tree](Tree/cartesian_tree.md) | コンパイル・実行成功 | エラー（対処は上表） |
| [永続セグメント木](DataStructureSegment/persistent_segment_tree.md) | コンパイル・実行成功 | 成功。分岐更新・境界探索・容量・巨大な添字も追加確認 |
| [第二種スターリング数](MathCombinatorics/stirling_second.md) | コンパイル・実行成功 | 成功 |
| [偏角ソート](Geometry/argument_sort.md) | コンパイル・実行成功 | 成功 |
| [Implicit Treap](DataStructureAdvanced/implicit_treap.md) | コンパイル・実行成功 | 成功 |
| [二項係数の累積和](MathCombinatorics/binomial_prefix_sum.md) | コンパイル・実行成功 | 成功 |
| [約数・倍数ゼータ／メビウス変換](MathNumberTheory/divisor_multiple_transforms.md) | コンパイル・実行成功 | 成功 |
| [Sorted Implicit Treap](DataStructureAdvanced/sorted_implicit_treap.md) | コンパイル・実行成功 | 成功 |
| [重み付きUnion-Find](DataStructureBasic/weighted_union_find.md) | コンパイル・実行成功 | 成功 |
| [LCA](Tree/lca.md) | コンパイル・実行成功 | 成功 |
| [Heavy-Light Decomposition](Tree/hld.md) | コンパイル・実行成功 | 成功 |
| [Rollback Union-Find](DataStructureBasic/rollback_union_find.md) | コンパイル・実行成功 | 成功 |
| [Li Chao Tree](DiscreteConvexity/li_chao_tree.md) | コンパイル・実行成功 | 成功 |
| [Convex Hull Trick](DiscreteConvexity/convex_hull_trick.md) | コンパイル・実行成功 | 成功 |
| [Deque CHT](DiscreteConvexity/deque_cht.md) | コンパイル・実行成功 | 成功 |
| [全方位木DP](Tree/rerooting.md) | コンパイル・実行成功 | エラー（対処は上表） |
| [Aho-Corasick](String/aho_corasick.md) | コンパイル・実行成功 | 成功 |
| [Mo法](Search/mo.md) | コンパイル・実行成功 | 成功 |
| [XOR基底](LinearAlgebra/xor_basis.md) | コンパイル・実行成功 | 成功 |
| [Miller-Rabin / Pollard Rho](MathNumberTheory/miller_rabin_pollard_rho.md) | コンパイル・実行成功 | 成功 |
| [SCCと縮約DAG](Graph/scc_condensation.md) | コンパイル・実行成功 | 成功 |
| [木の直径・最遠距離](Tree/tree_diameter.md) | コンパイル・実行成功 | 成功 |
| [静的Rolling Hash](String/static_rolling_hash.md) | コンパイル・実行成功 | 成功 |
| [Wavelet Matrix](DataStructureSegment/wavelet_matrix.md) | コンパイル・実行成功 | 成功 |
| [動的重み付きWavelet Matrix](DataStructureSegment/dynamic_weighted_wavelet_matrix.md) | コンパイル・実行成功 | 成功 |
| [動的Rolling Hash](String/dynamic_rolling_hash.md) | コンパイル・実行成功 | 成功 |
| [木の重心](Tree/tree_centroid.md) | コンパイル・実行成功 | 成功 |
| [XOR畳み込み（FWHT）](FPSandPolynomial/xor_convolution.md) | コンパイル・実行成功 | 成功 |
| [Segment Tree Beats](DataStructureSegment/segment_tree_beats.md) | コンパイル・実行成功 | 成功 |
| [Segment Tree Beats の拡張（改訂版）](DataStructureSegment/segment_tree_beats_extra.md) | コンパイル・実行成功 | 成功。ソース修正不要。境界・ランダム比較も確認 |
| [拡張ユークリッド互除法](MathNumberTheory/extended_euclid.md) | コンパイル・実行成功 | 成功 |
| [座標圧縮（改訂版）](DataStructureBasic/coordinate_compression.md) | コンパイル・実行成功 | 成功。回帰・ランダム・複数型・比較器も確認 |
| [二分探索の穴埋めテンプレート](Snippets/binary_search.md) | コンパイル・実行成功 | 成功（穴埋めを記入） |
| [modintのみをincludeするテンプレート](Templates/modint_only.md) | コンパイル・実行成功 | 成功（mainを記入） |
| [Functional Graph](Graph/functional_graph.md) | コンパイル・実行成功 | 成功 |
| [オイラー路・閉路](Graph/euler_trail.md) | コンパイル・実行成功 | 成功 |
| [線形篩](MathNumberTheory/linear_sieve.md) | コンパイル・実行成功 | 成功 |
| [Binary Lifting](Graph/binary_lifting.md) | コンパイル・実行成功 | 成功 |
| [ガウス消去法](LinearAlgebra/gaussian_elimination.md) | コンパイル・実行成功 | 成功 |
| [GF(2) のガウス消去法](LinearAlgebra/gf2_gaussian_elimination.md) | コンパイル・実行成功 | 成功 |
| [Virtual Tree](Tree/virtual_tree.md) | コンパイル・実行成功 | 成功（HLDの定義を併用） |
| [DSU on Tree](Tree/dsu_on_tree.md) | コンパイル・実行成功 | 成功 |
| [動的連結性（オフライン）](Graph/offline_dynamic_connectivity.md) | コンパイル・実行成功 | エラー（対処は上表） |
| [二進文字列の順序管理](String/binary_string_order_set.md) | コンパイル・実行成功 | 成功 |
| [Trie（英小文字）](String/trie_lowercase.md) | コンパイル・実行成功 | 成功 |
| [Trie（英小文字・英大文字）](String/trie_letters.md) | コンパイル・実行成功 | 成功 |
| [Trie（数字）](String/trie_digits.md) | コンパイル・実行成功 | 成功 |
| [Trie（英小文字・数字）](String/trie_lowercase_digits.md) | コンパイル・実行成功 | 成功 |
| [ACL segtree の各種ラッパー](DataStructureSegment/segtree_wrappers.md) | コンパイル・実行成功 | 成功 |
| [ACL segtree の穴埋めテンプレート](Snippets/segtree_custom.md) | コンパイル・実行成功 | 成功（穴埋めを記入） |
| [ACL lazy_segtree の各種ラッパー（改訂版）](DataStructureSegment/lazy_segtree_wrappers.md) | コンパイル・実行成功 | 成功。ソース修正不要。全17種類・境界探索も確認 |
| [ACL lazy_segtree の穴埋めテンプレート](Snippets/lazy_segtree_custom.md) | コンパイル・実行成功 | 成功（穴埋めを記入） |
| [複数テストケース用テンプレート](Templates/multiple_test_cases.md) | コンパイル・実行成功 | 成功（mainを記入） |
| [Codeforces用テンプレート](Templates/codeforces.md) | コンパイル・実行成功 | 成功（mainを記入） |
| [整数論のまとめ](MathNumberTheory/number_theory_bundle.md) | コンパイル・実行成功 | 成功 |
| [Binary Trie](DataStructureBasic/binary_trie.md) | コンパイル・実行成功 | 成功 |
| [LowLink](Graph/lowlink.md) | コンパイル・実行成功 | 成功 |
| [基本テンプレート](template.md) | コンパイル・実行成功 | 成功（mainを記入） |

## 確認範囲

掲載例に含まれるassertと、上記の個別不具合の再現を確認しました。改訂版RangeSet・座標圧縮・遅延セグ木・Beats拡張には追加で回帰・ランダム比較・型の端・サニタイザ検証を行っています。素数データは全78,498個の初期化子をC++23で使用し、個数・先頭・末尾を確認しています。
全API・全境界値・ランダム入力・性能・オーバーフローを網羅する検証ではありません。
別バージョンのGCC、Clang、MSVCやAtCoderの実行環境で同じ結果になるかは、この確認結果に含めていません。
