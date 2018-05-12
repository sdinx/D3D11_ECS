//----------------------------------------------------------------------------------
// D3D11_ECS
// create date: 2018.05.12 ( Prototype_version )
//----------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------
// Component
//----------------------------------------------------------------------------------
// namespace D3D11Utility
// 命名規則 基本的には自由 末尾にComponentを付ける場合は必ず統一する ( 例: WeaponComponent ).


/* コンポーネントについて */
// Entity.h のD3D11Utility::Entity クラスは一つに対して重複しない複数のコンポーネントを所持する.
// コンポーネントの実体は ComponentManager.h の D3D11Utility::Systems::ComponentManager クラスが持っている.
// コンポーネント自身の親のエンティティIDから他のコンポーネントを参照することができる.
// ただし, コンポーネントのつけ外しが頻繁に行われるため, 必ずnullチェックをしてコンポーネントが存在するかの確認をする必要がある.


/* コンポーネントの扱い方など */
// Entity::AddComponent関数でエンティティに対してコンポーネントを追加する ( 例: entity->AddComponent<Camera>() ).
// AddComponent関数の引数は追加するコンポーネントのコンストラクタ引数.
// Entity::GetComponent関数でエンティティのコンポーネントを取得できる ( 例: entity->GetComponent<Transform>() ).
// GetComponent関数は失敗すると nullptr が返る為コンポーネントの存在の確認ができる.


/* コンポーネントの実装方法について */
// Component.h をincludeしてD3D11Utility::Component クラスを継承.
// 必ずstatic ComponentId型で STATIC_COMPONENT_ID 変数を宣言する.
// Component.h で宣言してある 純粋仮想関数の実装とstatic関数としてSTATIC_COMPONENT_ID のゲッターとセッターが必要.
// Update関数に毎ループ行う処理を記述する.


/* 純粋仮想関数 */
// virtual  void  HandleMessage( const  GameUtility::Message&  msg ) = 0;
// 特定のメッセージを生成して分岐処理をさせる.
// virtual  void  HandleMessage( const  GameUtility::Message&  msg, Value  var ) = 0;
// 上記に引数として特定の値が必要な場合 ( void* 型からキャストする必要がある ).
// virtual  void  Update() = 0;
// 更新処理としてシステムが毎ループ呼び出す.


/* 参考コンポーネント */
// Renderable.h
// Camera.h

////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------
// ISystem と SystemManager
//----------------------------------------------------------------------------------
// namespace D3D11Utility::Systems
// 命名規則 末尾にSystemを付ける ( 例: PlayerSystem )


/* 実装したコンポーネントを管理するクラスについて */
// あくまでもシステム自身が管理するコンポーネントだけを扱うようにする.
// テンプレート関数のComponentManager::GetComponents関数を使ってコンポーネントを取得する.
// システムクラスのインスタンスの作成は必ず SystemManager::AddSystem関数 から行う.
// 同一システムを重複追加しては禁止.


/* コンポーネント管理クラスの実装について */
// ISystem.h をincludeしてD3D11Utility::Systems::ISystem クラスを継承する.
// Update関数に毎ループ行う処理と管理しているコンポーネントのUpdate関数呼び出す.


/* 参考システム */
// IDirect3DRenderer.h
// IDirect3DRenderer::Rendering関数 を参考にコンポーネントマネージャーからコンポーネントを取得する.