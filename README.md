# yuki4869 Algorithm Library — サイト管理

ACLの説明書を参考にした、競技プログラミング用C++ライブラリの参照サイトです。
分類・関数名による検索、ページ内目次、実装表示、コピー、ピン留めに対応します。

サイト: https://yuki-4869.github.io/site-73edf3ea04ba3512427ab7136f4894b4/

## リポジトリの役割

| リポジトリ | 内容 | 編集 |
| --- | --- | --- |
| yuki-4869/Algorithm-Private | 移行前のコード・説明・履歴の非公開保存 | 更新しない |
| yuki-4869/Algorithm | 公開するC++ソースコード64本から開始 | コードの編集元 |
| このリポジトリ | 説明文、サイト、検索、同期・公開処理 | 説明と表示を編集 |

公開用Algorithmには、README、説明ページ、サイトへの案内、同期用ワークフローを追加しません。
素数表はソースコードではないため、サイト側の補助データとして管理します。
保存用リポジトリへの書き込みは行いません。

## 自動同期

サイト側のGitHub Actionsが、公開用Algorithmのmainを取得してページを生成します。

- このリポジトリのmain更新時: 再生成・公開。
- 定期確認: 毎時7・22・37・52分（UTC）。コード更新があれば再生成・公開。
- 手動: Actionsの「Sync code and deploy library」から「Run workflow」。

定期実行はGitHub側の都合で遅延することがあります。
公開リポジトリの定期ワークフローは、リポジトリに60日間活動がないと無効になる場合があります。
長期休止後はActionsで再有効化してください。
このリポジトリのpush、または手動実行でも公開できます。

コードの追加・変更・削除は、次の生成でサイトに反映されます。
コードをこのリポジトリへ手作業で複製する必要はありません。
同期はAlgorithmからサイトへの一方向です。
追加のPATや同期用Secretsは不要です。サイトの公開は自身のGITHUB_TOKENとOIDCを使用します。

## ライブラリ追加・説明の更新

1. AlgorithmにC++コードを追加・更新します。
2. このリポジトリのcatalog.jsonに、表示名・分類・概要を追記します。
3. content/usage/に、コードと同じフォルダ・名前のMarkdown説明を作成します。
4. 説明と実装の一致を確認し、次のコマンドで対応するコードの版を記録します。

    SOURCE_DIR=source node scripts/mark-reviewed.mjs DataStructureBasic/range_set.cpp

5. 説明・catalog・documentation-revisions.jsonを一緒にcommitします。

説明が未作成の新しいコードにも、実装を閲覧できるページを自動で作成します。
APIや計算量は自動では推測しません。
説明が対応する版とコードが異なる場合は、説明ページの冒頭に更新確認の案内を表示します。

## ローカル生成

Node.js 24を使用します。

    git clone https://github.com/yuki-4869/Algorithm.git source
    npm ci
    npm run build
    npm run check

SOURCE_DIRでソースの取得先を変更できます。
Git履歴のないディレクトリから生成する場合は、SOURCE_SHAに公開コードの40桁コミットIDを指定します。
dist/は生成物で、mainには保存しません。

## AtCoderでの参照

「参照URLをコピー」は、画面に表示している実装と同じコミットの公開コードURLをコピーします。
2026年10月14日版のAtCoder生成AI対策ルールでは、生成AIを用いて作成されたコードは、
コンテスト開始前のWeb公開と、コピーして使用する際の参照URLの添付が必要です。
新規公開・更新したコードは、公開後に開始するコンテストで使用してください。

- 公式ルール: https://info.atcoder.jp/entry/llm-rules-ja
- 固定URL: https://docs.github.com/en/repositories/working-with-files/using-files/getting-permanent-links-to-files

## 公開範囲

このサイトはログイン制限のない公開サイトです。
ランダムなパスとnoindexは、アクセス制限ではありません。
公開コード側からサイトへのリンクは設けません。
GitHub上のサイト用リポジトリやActionsの情報から場所を知ることは可能です。

## 検証

サイト生成時に、全ページの内部リンク・目次アンカー・検索対象数・
ソースのSHA-256・Git blob SHA・参照URLのコミット一致を確認します。
移行時の64本は、保存用の同一ファイルとGit blob SHAが一致しています。
説明内の過去のコンパイル結果は当時の検証記録であり、毎回のサイト生成でコードを再検証するものではありません。

## 表示デザイン

ACL公式ドキュメントが使用するUIkit 3.5.16に合わせ、白背景、本文16px・行高1.5・文字色#666、見出し#333、リンク#1e87f0、同一のフォントスタックを使用します。本文は最大1200pxの中央配置です。分類メニューと目次は開閉式です。UIkit由来の基礎スタイルのライセンスはweb/UIKIT-LICENSE.txtに保存しています。
