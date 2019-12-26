（１）VMディレクトリにあるVirtualBox-5.2.18-124319-Win.exeをダブルクリックしOracle VM VirtualBoxをインストール
（２）VMディレクトリにあるLXDE_Ubuntu1804.7zを解凍
（３）Oracle VM VirtualBoxマネージャーを起動し「新規(N)」を押下
（４）名前を入力し、タイプはLinux、バージョンはUbuntu(64-bit)を選択し、「次へ(N)」を押下
（５）メモリサイズは4GB以上を設定し、「次へ(N)」を押下
（６）ハードディスクは「すでにある仮想ハードディスクを使用する」にチェックを入れ、（２）で回答したvdiイメージを選択
（７）「作成」を押下
（８）（７）で作成した仮想マシンを選択し、「設定(S)」を押下
（９）一般 -> 高度(a)タブを選択し、クリップボードの共有とドラッグ＆ドロップで双方向を選択
（１０）システム -> プロセッサー(P)タブを選択し、プロセッサ数を2以上に設定
（１１）ディスプレイ -> スクリーン(S)タブを選択し、ビデオメモリーを128MBに設定
（１２）共有フォルダで＋の付いたディレクトリアイコンを押下
（１３）共有フォルダの追加ダイアログでWindowsとLinuxで共有するフォルダを選択し、自動マウント(A)にチェック
（１４）共有フォルダの追加ダイアログで「OK」を押下
（１５）（８）で開いている「設定」ダイアログで「OK」を押下
（１６）「起動(T)」を押下し、Linuxを起動
（１７）Linuxが起動したら、パスワードmbpを入力してログイン
（１８）起動した仮想マシンのダイアログで「デバイス」->Guest Addtions CDイメージの挿入を選択
（１９）Removable medium is insertedのダイアログが表示されたら、「OK」を押下
（２０）LXTerminalを起動し、cd /media/mbp/VBox_GAs_5.2.18
（２１）（２０）でcdしたLXTerminalよりsudo ./VBoxLinuxAdditions.runを実行(sudoパスワードはmbp)
（２２）VirtualBox Guest Additions: Starting.が表示されたら、sudo usermod -G vboxsf -a mbpでvboxsfにアカウント登録
（２３）LXTerminalをexitし、仮想マシンをshutdown
（２４）（２３）でshutdownした仮想マシンを選択し、「設定(S)」を押下
（２５）ストレージ -> コントローラー：IDEで表示されているVBoxGuestAdditions.isoを削除
（２６）光学ドライブを削除しますか？というダイアログが表示されるので、「除去(R)」を押下
（２７）（２４）で表示した設定ダイアログの「OK」を押下
（２８）再度仮想マシンを起動
（２９）パスワードmbpを入力し、ログインしたら画面に共有ディレクトリフォルダが配置されているのでダブルクリック
（３０）エクスプローラーで問題なく表示されたらLinuxの設定は完了

以下は手動でProxy設定が必要な場合
＝ホストOS側(Windows)設定＝
（３１）仮想マシンを実行しているダイアログで「ファイル」->「環境設定」を選択
（３２）VirtualBox環境設定ダイアログが表示されるので、プロキシーを選択
（３３）手動プロキシー設定を選択し、ホストとポートを入力して「OK」を押下

＝ゲストOS側(Linux)設定＝
（３４）MBPToolchainInstallerディレクトリを共有フォルダにコピー
（３５）LXTerminalを起動し、MBPToolchainInstallerディレクトリにcd
（３６）./Set_1_Ubuntu1804Proxy.sh
（３７）（３３）で入力したホストとポートを入力
（３６）LXTerminalをexitし、仮想マシンをshutdown後、Oracle VM VirtualBoxマネージャーから再度起動
（３７）仮想マシンにログインしたらpingで外の世界と通信できていることを確認

