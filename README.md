This was an attempt to create an SQLite index of Wikipedia.

The Wikipedia dumps are organized by article ID rather than title so the idea was to build an external index to allow finding an article quickly by title.

However, at least with this first attempt, it seems that SQLite may not be practical for this. Even just for 10k articles, building an
index by title is taking around 9 minutes.

Let's instead for now look at data formats/DBs more specialized for this task -- fast lookup by string index.

# Requirements

This project uses Bazel for builds: https://bazel.build/.
