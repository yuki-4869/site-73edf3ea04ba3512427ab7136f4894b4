# Stern–Brocot 木

正の有理数を $1/1$ を根とする二分木で表します。経路を `L`・`R` の連長圧縮で持てば、深さが大きくても少ない操作で処理できます。

## API

`SternBrocot::Fraction` は `pair<long long,long long>` で、分子・分母の順です。`SternBrocot::Path` は `vector<pair<char,long long>>` で、方向と連続回数の組です。

| 操作 | 戻り値と内容 | 時間 |
| --- | --- | --- |
| `SternBrocot::encode(f)` | `Path`。根から `f` への圧縮経路 | $O(K+1)$ |
| `SternBrocot::decode(path)` | `Fraction`。経路の終点 | $O(K+1)$ |
| `SternBrocot::lca(f, g)` | `Fraction`。根からの経路の共通接頭辞の終点 | $O(K_f+K_g+1)$ |
| `SternBrocot::ancestor(f, d)` | `optional<Fraction>`。深さ $d$ の祖先。深すぎると `nullopt` | $O(K+1)$ |
| `SternBrocot::range(f)` | `pair<Fraction,Fraction>`。自身を含む部分木を表す開区間の下限と上限 | $O(K+1)$ |

$K$ は圧縮後の経路長です。`encode` は連続する移動を Euclid の互除法の商でまとめ、$K=O(\log(\max(a,b)+1))$ です。根の深さは 0。`range` は 0 を `{0,1}`、正の無限大を `{1,0}` で返します。

分数の入力は正の分子・分母を使います。約分前の入力も扱えます。経路の方向は `L` または `R`、各回数は正です。`decode` の結果は分子・分母とも `long long` に収まる必要があります。

## 使用例

```cpp
SternBrocot::Fraction f{5,3};
auto path=SternBrocot::encode(f);
if(SternBrocot::decode(path)!=f)std::abort();
if(SternBrocot::lca(f,{3,2})!=SternBrocot::Fraction{3,2})std::abort();
```

[Library Checker・Stern–Brocot 木](https://judge.yosupo.jp/problem/stern_brocot_tree)
