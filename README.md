# Titans

C++ と SFML 3.0 を使用した CPU 対戦型チェスゲーム

## 機能

- 完全なチェスルール実装
  - キャスリング（キングサイド / クイーンサイド）
  - アンパッサン
  - ポーンプロモーション
  - チェック / チェックメイト判定
  - ステイルメイト判定
  - 引き分け判定（駒不足）
- AI対戦（Minimax + Alpha-Beta枝刈り、探索深度4）
- Unicode チェス駒表示（♔♕♖♗♘♙ / ♚♛♜♝♞♟）

## 操作方法

| 操作 | 説明 |
|------|------|
| 左クリック | 駒を選択 / 移動先を指定 |
| Esc | 選択解除 |
| R | ゲームリスタート |

---

## セットアップ手順（Windows 11）

### 1. 前提条件のインストール

PowerShell を開いて以下を実行：

```powershell
# Git のインストール
winget install Git.Git

# CMake のインストール（インストール時に「Add CMake to PATH」を選択）
winget install Kitware.CMake --interactive

# Visual Studio Build Tools のインストール
winget install Microsoft.VisualStudio.2022.BuildTools
```

**重要**: Visual Studio Build Tools インストール時に **「C++によるデスクトップ開発」** ワークロードを選択してください。

インストール後、**PowerShell を再起動**してください。

### 2. インストール確認

```powershell
git --version
cmake --version
```

両方ともバージョンが表示されれば OK です。

### 3. vcpkg のインストール

```powershell
# C:\ に vcpkg をインストール（推奨）
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

**注意**: パスにスペースが含まれるフォルダ（例: `C:\Users\John Doe\`）へのインストールは避けてください。

### 4. SFML のインストール

```powershell
cd C:\vcpkg
.\vcpkg install sfml:x64-windows
```

これには数分かかります。

### 5. プロジェクトのビルド

```powershell
cd <プロジェクトフォルダ>
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

### 6. 実行

```powershell
cd Release
.\ChessTitans.exe
```

---

## プロジェクト構成

```
titans/
├── CMakeLists.txt
├── README.md
├── assets/
│   └── pieces/
└── src/
    ├── main.cpp
    ├── Game.cpp/h
    ├── Board.cpp/h
    ├── Piece.cpp/h
    ├── Move.cpp/h
    ├── AI.cpp/h
    ├── Renderer.cpp/h
    └── Pieces/
        ├── King.cpp/h
        ├── Queen.cpp/h
        ├── Rook.cpp/h
        ├── Bishop.cpp/h
        ├── Knight.cpp/h
        └── Pawn.cpp/h
```

---

## 改善ポイント

### 機能面

| 改善項目 | 説明 |
|----------|------|
| **難易度選択** | AI の探索深度を変更可能にする（Easy: 2、Normal: 4、Hard: 6） |
| **手の履歴表示** | 画面横に指し手の履歴を表示（代数記法: e2-e4 など） |
| **Undo/Redo 機能** | 手を戻す・進める機能の追加 |
| **駒画像の使用** | Unicode 文字の代わりに PNG 画像を使用してより見やすく |
| **サウンド効果** | 駒を動かす音、チェック時の警告音など |
| **対人プレイモード** | CPU ではなく2人対戦モードの追加 |
| **オープニングブック** | 序盤の定跡データベースを使用して序盤を強化 |
| **タイマー機能** | 持ち時間制限の追加（対局時計） |
| **棋譜の保存/読込** | PGN 形式での棋譜の保存と読み込み |
| **プロモーション UI** | プロモーション時の駒選択 UI の改善 |

### 性能面

| 改善項目 | 説明 |
|----------|------|
| **トランスポジションテーブル** | Zobrist ハッシュを使用して同一局面の再計算を防止 |
| **反復深化** | 時間制限内で可能な限り深く探索する |
| **キラーヒューリスティック** | 良い手を優先的に探索して枝刈り効率を向上 |
| **ヒストリーヒューリスティック** | 過去に良かった手を優先的に探索 |
| **Null Move Pruning** | パスして相手に手番を渡しても優勢なら枝刈り |
| **Late Move Reduction** | 後半の手の探索深度を減らして高速化 |
| **ビットボード** | 64bit 整数で盤面を表現して高速化 |
| **マルチスレッド探索** | AI の思考を別スレッドで実行して UI をブロックしない |
| **評価関数の改善** | キングの安全性、ポーン構造、駒の連携などを評価 |
| **エンドゲームテーブル** | 終盤の完全解析データを使用 |

### コード品質

| 改善項目 | 説明 |
|----------|------|
| **ユニットテスト** | Google Test などでチェスルールのテストを追加 |
| **Perft テスト** | 合法手生成の正確性を検証 |
| **設定ファイル** | AI の深度や画面サイズを外部ファイルで設定可能に |
| **ログ機能** | デバッグ用のログ出力機能 |

---

## 技術仕様

- **言語**: C++17
- **グラフィックス**: SFML 3.0
- **ビルドシステム**: CMake 3.16+
- **AI アルゴリズム**: Minimax with Alpha-Beta Pruning
- **探索深度**: 4（調整可能）
- **評価関数**: 駒の価値 + Piece-Square Tables

### 駒の価値

| 駒 | 価値 |
|----|------|
| Pawn | 100 |
| Knight | 320 |
| Bishop | 330 |
| Rook | 500 |
| Queen | 900 |
| King | 20000 |

---

## トラブルシューティング

### CMake が見つからない

```
'cmake' は、コマンドレット、関数、スクリプト ファイル、...として認識されません
```

**解決策**: CMake を再インストールし、インストール時に「Add CMake to PATH」を選択。その後 PowerShell を再起動。

### vcpkg.cmake が見つからない

```
Could not find toolchain file: "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

**解決策**: vcpkg のインストールパスを確認し、`-DCMAKE_TOOLCHAIN_FILE=` のパスを修正。

### Unicode 文字のコンパイルエラー

```
warning C4819: ファイルは、現在のコード ページ (932) で表示できない文字を含んでいます
```

**解決策**: CMakeLists.txt に以下が含まれていることを確認：
```cmake
if(MSVC)
    add_compile_options(/utf-8)
endif()
```

---

## ライセンス

MIT License
