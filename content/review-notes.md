# ライブラリの確認事項

確認日: 2026-10-15（日本時間）

初回整理時の記録と、その後に確認・修正した事項をまとめています。全実装の正しさを保証するテスト結果ではありません。

## RangeSetの改訂（2026-10-15）

[改訂コード](https://github.com/yuki-4869/Algorithm/blob/56ecc78753f54a85b653e23783dcab7ea0c48eb6/DataStructureBasic/range_set.cpp) · [新しい使い方](usage/DataStructureBasic/range_set.md) · [テスト](../tests/range_set_test.cpp)

- 内部を番兵なしのmapに変更し、隣接区間を必ず統合。
- eraseのコンパイルエラー、部分削除の返り値・総被覆数、covered/mexの不一致を修正。
- 最小・最大座標付近での演算を確認し、被覆数のオーバーフローを変更前に検出。
- optionalでの保持区間取得、重なり判定、clear、読み取り専用走査を追加。
- C++23で524,288操作を独立な整数集合と比較。AddressSanitizer・UBSan・デバッグイテレータでの実行も成功。

主要APIの呼び方は維持しています。内部フィールドへの直接アクセスとsizeの返り値型の変更については、使い方ページの「旧版からの移行」を参照してください。

## 座標圧縮の改訂（2026-10-15）

[改訂コード](https://github.com/yuki-4869/Algorithm/blob/0eacd5e38b1bf98e3870a77821d54cee82f011e0/DataStructureBasic/coordinate_compression.cpp) · [新しい使い方](usage/DataStructureBasic/coordinate_compression.md) · [専用テスト](../tests/coordinate_compression_test.cpp)

- namespace定義前のusing宣言を修正し、必要な標準ヘッダもソースに収録。
- 未構築状態・範囲外番号を、NDEBUGでも例外で検出。
- get/zipの挿入位置を返す互換動作を維持し、index/zip_exactで厳密な登録済み番号も取得可能。
- 逆変換、読み取り専用の表・列挙、種類数の区間集計、clear・assign等を追加。
- 比較器の等価性で重複を除去し、operator==がない型も扱えるように変更。
- 全公開APIの返り値・操作・計算量を末尾コメントに記載。
- GNU C++23で131,072ステップのランダム比較、複数の型・比較器・境界・コピー・ムーブを確認。NDEBUG・AddressSanitizer・UBSan・デバッグイテレータでも成功。

xs / builtへの直接アクセスと、未構築時のsizeの利用については、新しい使い方の移行表を参照してください。

## 遅延セグ木・Beats拡張の改訂（2026-10-15）

[遅延セグ木の改訂コード](https://github.com/yuki-4869/Algorithm/blob/f9177dec185cbc540c81fa7c94c944c1e87348ac/DataStructureSegment/lazy_segtree_wrappers.cpp) · [Beats拡張の改訂コード](https://github.com/yuki-4869/Algorithm/blob/f9177dec185cbc540c81fa7c94c944c1e87348ac/DataStructureSegment/segment_tree_beats_extra.cpp) · [専用テスト](../tests/segment_wrappers_test.cpp)

- 遅延セグ木の開始コメント欠落、Beats拡張のmin/max名前解決を修正。ソースを書き換えず新旧の使用例が通ることを確認。
- 遅延セグ木17種類の既定・要素数・配列による構築を整え、空状態・境界探索・単位元への更新を修正。
- Beats拡張4種類に境界・入力検査を追加し、除数1の処理と除算・平方根の一括更新を改善。AND/ORは非負long longの下位63bitに対応。
- コピー・ムーブを整え、ムーブ元は長さ0として利用可能。範囲や不正入力の検査はNDEBUGでも有効。
- 返り値・操作・計算量を全公開APIの末尾コメントと説明書へ記載。非線形更新は単発最悪O(N)であることを明記。
- GNU C++23で163,840グループステップを通常の配列と比較。NDEBUG・AddressSanitizer・UBSan・デバッグイテレータでも成功。

LazyAffineSumの既定modは998244353です。別mod、内部フィールド、SegSqrt::Nodeの変更は、[遅延セグ木](usage/DataStructureSegment/lazy_segtree_wrappers.md)・[Beats拡張](usage/DataStructureSegment/segment_tree_beats_extra.md)の移行表を参照してください。整数の中間結果・和がlong longに収まることは呼び出し側の制約です。

## 永続セグメント木の改訂（2026-10-22）

[使い方](usage/DataStructureSegment/persistent_segment_tree.md) · [専用テスト](../tests/persistent_segment_tree_test.cpp)

- 番兵生成時の未初期化ポインタ参照を除去。ノードをdequeへ必要な分だけ確保し、追加前に容量を検査。
- 木本体のコピー・ムーブを禁止し、二重解放や他の本体への内部ポインタ混在を防止。根ポインタによるO(1)の版の共有は維持。
- N=0、buildの長さ、添字・区間、nullptr、探索述語の単位元検査を追加。検査はNDEBUGでも有効で、違反時はメッセージなしで停止。
- 中点を `l+(r-l)/2` に変更し、N=LLONG_MAXの疎な木でも更新・取得・境界探索を確認。
- 三値列の全列挙、64,000回の過去の版からの分岐、非可換演算とmax_right、容量境界、デフォルト構築できない型、破棄を専用テストへ追加。

基本APIは維持しています。内部フィールドへの直接アクセスとコピー・ムーブ禁止については、使い方ページの「入力検査と旧版からの移行」を参照してください。

## 優先して確認・修正する項目

C++23（GCC 13.3.0）での確認結果に基づく、残っている項目の優先順です。RangeSet・座標圧縮・遅延セグ木ラッパー・Beats拡張・永続セグメント木は改訂で修正済みです。

| 優先度 | 対象 | 理由・次に行うこと |
| --- | --- | --- |
| **高** | [オンライン動的連結性](usage/DataStructureAdvanced/dynamic_connectivity.md)、[オフライン動的連結性](usage/Graph/offline_dynamic_connectivity.md)、[Cartesian Tree](usage/Tree/cartesian_tree.md) | 全角空白、基本テンプレートのpマクロとの衝突、グローバルの未宣言vでコンパイル不可。説明書の対処をコードへ反映する |
| **負数を扱うなら高** | [約数列挙](usage/MathNumberTheory/divisors.md) | get_divisors(-4)で±2が重複する。絶対値に対する平方判定へ直し、負の完全平方数を確認する |
| **使用例を整備** | [Functional Graph](usage/Graph/functional_graph.md)、[全方位DP](usage/Tree/rerooting.md) | 前者の末尾コメントは実装にないAPIを掲載。後者は未記入のidentity()を含む。使い方コメントを実際のAPI・完成例へ合わせる |

次は **コピー時のコンパイルエラー群** の確認を推奨します。
詳しい再現例と確認範囲は [C++23での確認結果](usage/validation.md) を参照してください。

## 初回整理時に確認した事項

### 旧座標圧縮の宣言順（修正済み）

旧版の [coordinate_compression.cpp](../DataStructureBasic/coordinate_compression.cpp) の先頭に
`using coordinate_zip::Zip;` がありますが、`coordinate_zip` はその後で初めて宣言されています。
標準ヘッダと `using namespace std;` を用意して `g++ -std=c++20 -fsyntax-only` で確認したところ、
`coordinate_zip has not been declared` に相当するエラーが出ました。
旧版ではusing宣言をnamespace定義の後へ移す必要がありました。現行版では修正済みで、ソース全体をそのまま使用できます。

### 負の完全平方数の約数列挙

[divisors.cpp](../MathNumberTheory/divisors.cpp) の `get_divisors(-4)` は次の結果になります。

```text
-4 -2 -2 -1 1 2 2 4
```

`i*i != n` の比較は負数に対して常に真になるため、負の完全平方数で同じ約数が重複します。
正数4については `1 2 4` を確認しました。負数に対応する場合は絶対値との比較などを検討してください。
配布コードにはまだこの変更を加えていません。

### 素数データ

[primes_upto_1000000.txt](../MathNumberTheory/primes_upto_1000000.txt) の整数列を独立に生成した篩の結果と比較し、
10^6 以下の素数78,498個と完全に一致することを確認しました。先頭は2、末尾は999983です。

## テンプレート・依存関係

- [template.cpp](../template.cpp) と [multiple_test_cases.cpp](../Templates/multiple_test_cases.cpp) は、関数本体の途中で終わる追記用テンプレートです。問題の処理と閉じ括弧を追記して使います。
- [binary_search.cpp](../Snippets/binary_search.cpp) は `ok`・`ng`・`check` の未記入部分があります。更新の向きと終了条件も問題に合わせて確認します。
- [segtree_custom.cpp](../Snippets/segtree_custom.cpp) と [lazy_segtree_custom.cpp](../Snippets/lazy_segtree_custom.cpp) はモノイド・写像を定義して使う穴埋めテンプレートです。
- LazyXXX は [lazy_segtree_wrappers.cpp](../DataStructureSegment/lazy_segtree_wrappers.cpp) の `LazySegBase` に依存します。
- [modint_only.cpp](../Templates/modint_only.cpp) は元のファイル名が「コードatcoder抜き」ですが、`#include <atcoder/modint>` とmodintへの依存があります。
- `Graph` の型はテンプレートの別版で異なります。基本テンプレートは `vector<vector<pair<ll,ll>>>`、codet/codecf由来の別版は `vector<pair<ll,ll>>` です。
- 2つのオフライン動的連結性コードは同名の `OfflineDynamicConnectivity` を、整数論の2つのファイルは同名の `LinearSieve` を含みます。同時に貼り付ける場合は名前の衝突を避けます。
- [binomial_prefix_sum.cpp](../MathCombinatorics/binomial_prefix_sum.cpp) は、共通テンプレートの `nCr`・`mint`・`rep` に依存します。
- [palindrome_radius.cpp](../String/palindrome_radius.cpp) は関数内に置き、処理対象の `S` を設定して使います。文字を中心とする奇数長回文が対象です。

## ファイル名を本文に合わせた例

| 元の名前 | 整理後 | 根拠 |
| --- | --- | --- |
| トポロジカルソート | [scc_condensation.cpp](../Graph/scc_condensation.cpp) | 本文は SCCResult と縮約DAGの処理 |
| 素数判定 | [number_theory_bundle.cpp](../MathNumberTheory/number_theory_bundle.cpp) | 篩・素数関連・約数／倍数変換などを含む |
| TrieAZ | [trie_lowercase.cpp](../String/trie_lowercase.cpp) | 文字写像は `c-'a'` |
| TrieAZ09 | [trie_lowercase_digits.cpp](../String/trie_lowercase_digits.cpp) | 英小文字と数字を扱う |
| 01-StringWavelerMatrix | [binary_string_order_set.cpp](../String/binary_string_order_set.cpp) | 二進文字列を扱い、本文のクラス名は StringWaveletMatrix |

## 今回の検証範囲

- 入力65ファイルの保持と、各ファイルのバイト列・SHA-256の一致。
- Markdown内の相対ファイル／フォルダリンクの存在。
- 上記の座標圧縮の構文エラー、約数列挙の具体例、素数データの照合。

この節は初回整理時の範囲を記録したものです。追加のC++23・ACLでの使用例検証は [現在の確認結果](usage/validation.md) を参照してください。

## 詳細ドキュメント作成時の追加確認（2026-10-15）

[65ファイルの使い方](usage/index.md) と [使用例の確認結果](usage/validation.md) を追加しました。
この節は最初に詳細ドキュメントを作成した際の記録です。その後、RangeSet・座標圧縮・遅延セグ木ラッパー・Beats拡張は改訂で修正しています。

| 対象 | 確認事項 |
| --- | --- |
| [Functional Graph](usage/Graph/functional_graph.md) | 末尾コメントにあるjump/distance等は未実装。実際のget_* APIを説明 |
| [旧RangeSet（改訂済み）](usage/DataStructureBasic/range_set.md) | 旧版でeraseのコンパイルエラー、隣接区間の被覆・mexと削除数の不一致を再現。現在は修正済み |
| [オンライン動的連結性](usage/DataStructureAdvanced/dynamic_connectivity.md) | コメント外の全角空白がコンパイルエラー |
| [オフライン動的連結性](usage/Graph/offline_dynamic_connectivity.md) | 出力マクロpと内部vector pの初期化が衝突 |
| [旧Segment Tree Beats Extra（修正済み）](usage/DataStructureSegment/segment_tree_beats_extra.md) | 補助処理のmin/maxが同名メンバに隠されていた。現行版では修正済み |
| [旧遅延セグ木ラッパー（修正済み）](usage/DataStructureSegment/lazy_segtree_wrappers.md) | 末尾説明文の開始コメントが欠落していた。現行版では修正済み |
| [Cartesian Tree](usage/Tree/cartesian_tree.md) | 末尾の使用例がコメント外。クラス定義と呼び出し部分を分ける |
| [全方位DP](usage/Tree/rerooting.md) | 問題固有のDP/Eと演算の未記入例を含む。完成例を別途掲載 |

以前の「検証範囲」は初回整理時の範囲です。今回の使用例検証については上記リンクを参照してください。
