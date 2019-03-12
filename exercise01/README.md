# Exercise Sheet 1

In this exercise you will be asked to work with a shell. The idea is to get
familiar with some Unix commands and bash programming.

## Task 1

Consider the following scripts. Explain what each line means within these
scripts. Explain what happens when you execute these scripts with and without
input arguments.

### Script 1

```bash
#!/bin/bash
for FN in "$@"
do
    chmod 0750 "$FN"
done
```

### Script 2

```bash
#!/bin/bash
function usage {
    echo "$0: <TODO: fill me in>"
    exit 1
}

ARG1=$1; shift || usage
ARG2=$1; shift || usage
ARG3=$1; shift || usage

grep -n "$ARG3" "$ARG1" > "$ARG2"
```

Hint: `shift` is a shell _buit-in_, use `help shift` to get more information.

## Task 2

What does this shell script do? Improve its usability by adding error
handling for the following cases:

- Print a help message when the number of provided arguments is not two.
- Log an error message to a file `error.log` when the file `$OUTFILE` is not writable.

```bash
#!/bin/bash
INFILE=$1
OUTFILE=$2
if [ -e "$INFILE" ]
then
    cat "$INFILE" >> "$OUTFILE"
fi
```

Hint: Take a look at the man page for the "test" command: `man "["`.

## Task 3

Write a script **my_backup.sh** to perform a backup of the current directory
(containing only files) into a given folder. The script receives a single
parameter which is the path of the folder where the backup is to be stored.
If the backup folder already exists then a file must be copied if and only if
it does not exist within the backup folder or if it is newer than the
existing one in the backup folder. The script will be used as follows

```bash
bash my_backup.sh /path/to/backup-folder # creates a backup of . into backup-folder
```

**Hint**: Use `stat --format %Y <file>` to get the modification time of a file.
