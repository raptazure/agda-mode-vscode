open Belt;
open Guacamole.Vscode;

// a dictionary of FileName-State entries
module StateDict = {
  module Impl = (Editor: Sig.Editor) => {
    module State = State.Impl(Editor);
    let dict: Js.Dict.t(State.t) = Js.Dict.empty();

    let get = fileName => dict->Js.Dict.get(fileName);

    let getByEditor = (editor: Editor.editor) =>
      editor->Editor.getFileName->Option.flatMap(get);

    // do nothing if the state already exists
    let add = (fileName, state) => {
      switch (get(fileName)) {
      | Some(_) => ()
      | None => dict->Js.Dict.set(fileName, state)
      };
    };

    let rename = (oldName, newName) => {
      let delete_: (Js.Dict.t('a), string) => unit = [%raw
        "function (dict, key) {delete dict[key]}"
      ];
      get(oldName)
      ->Option.forEach(state => {
          Js.log3("[ states ][ rename ]", oldName, newName);
          delete_(dict, oldName);
          add(newName, state);
        });
    };

    // remove the entry (but without triggering .destroy() )
    let remove = fileName => {
      let delete_: (Js.Dict.t('a), string) => unit = [%raw
        "function (dict, key) {delete dict[key]}"
      ];
      delete_(dict, fileName);
    };
    let destroy = fileName => {
      get(fileName)
      ->Option.forEach(state => {
          Js.log("[ states ][ destroy ]");
          State.destroy(state) |> ignore;
        });
      remove(fileName);
    };

    let contains = fileName => get(fileName)->Option.isSome;

    let destroyAll = () => {
      dict
      ->Js.Dict.entries
      ->Array.map(((_, state)) => State.destroy(state))
      ->ignore;
    };
  };
};

module Impl = (Editor: Sig.Editor) => {
  module States = StateDict.Impl(Editor);
  module State = State.Impl(Editor);
  module TaskCommand = Task__Command.Impl(Editor);
  module TaskRunner = TaskRunner.Impl(Editor);

  let addToSubscriptions = (f, context) =>
    f->Js.Array.push(context->ExtensionContext.subscriptions)->ignore;

  let activate = context => {
    // when a TextEditor gets closed, destroy the corresponding State
    Editor.onDidCloseEditor(States.destroy)
    ->Editor.addToSubscriptions(context);
    // when a file got renamed, destroy the corresponding State if it becomes non-GCL
    Editor.onDidChangeFileName((oldName, newName) =>
      oldName->Option.forEach(oldName =>
        newName->Option.forEach(newName =>
          if (States.contains(oldName)) {
            if (Editor.isAgda(newName)) {
              States.rename(oldName, newName);
            } else {
              States.destroy(oldName);
            };
          }
        )
      )
    )
    ->Editor.addToSubscriptions(context);

    // on editor activation, reveal the corresponding Panel (if any)
    Editor.onDidChangeActivation((prev, next) => {
      //   prev->Option.flatMap(States.get)->Option.forEach(State.hide);
      //   next->Option.flatMap(States.get)->Option.forEach(State.show);
      ()
    })
    ->Editor.addToSubscriptions(context);

    // on trigger command
    Command.names->Array.forEach(((command, name)) => {
      Editor.registerCommand(
        name,
        editor => {
          // special treatments on commands like "Load" and "Quit"
          switch (command) {
          | Load =>
            editor
            ->Editor.getFileName
            ->Option.forEach(fileName => {
                switch (States.get(fileName)) {
                | None =>
                  // not in the States dict, instantiate one new
                  let state = State.make(context, editor);
                  // remove it from the States dict if it got destroyed
                  state
                  ->State.onDestroy(() => {States.remove(fileName)})
                  ->Editor.addToSubscriptions(context);
                  States.add(fileName, state);
                | Some(_state) =>
                  // already in the States dict, do nothing
                  ()
                }
              })
          | Quit =>
            editor
            ->Editor.getFileName
            ->Option.forEach(fileName => {
                switch (States.get(fileName)) {
                | None =>
                  // not in the States dict, do nothing
                  ()
                | Some(_state) =>
                  // already in the States dict, remove it
                  fileName->States.destroy
                }
              })
          | _ => ()
          };
          // dispatch Tasks
          editor
          ->States.getByEditor
          ->Option.forEach(state => {
              Js.log(state);
              TaskCommand.dispatch(command)
              |> TaskRunner.run(state)
              |> ignore;
            });
        },
      )
      ->Editor.addToSubscriptions(context)
    });
  };

  let deactivate = () => {
    States.destroyAll();
  };
};