This console-based C++ application simulates the core editing functionalities used in a press agency. 
It allows inserting and deleting text and supports undo and redo operations using a custom stack data structure.

The core idea is to digitize the editing process, making text changes reversible.

## Features

- `INSERT <index> <text>`: Insert a string at the given position.
- `DELETE <index> <length>`: Delete a substring starting from index with the specified length.
- `UNDO`: Undo the last performed operation (INSERT or DELETE).
- `REDO`: Reapply the last undone operation.
- `EXIT`: Exit the program.

The undo and redo functionalities are implemented using two stacks that store operations performed.

Each operation is stored in a structure that includes:
- Operation type: INSERT or DELETE
- Position: the index in the text
- Text: the text inserted or deleted

## Example Workflow

```
INSERT 0 Article
# Text becomes: "Article"
DELETE 1 2
# Deletes "rt", text becomes: "Aicle"
UNDO
# Reverts delete, text becomes: "Article"
REDO
# Reapplies delete, text becomes: "Aicle"
```

## Input Validation

- Handles incorrect inputs (e.g., negative indices, index out of bounds).
- Notifies user when UNDO or REDO cannot be performed.

## Notes

- Indexes are 0-based.
- Each valid operation updates the text and prints the result to the console.
