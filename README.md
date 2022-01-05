# Playlist in C
 - This program was developed by a group of friends in the 3rd semester of college, where we used data structures to simulate a playlist.
 
## Development:
 * This application uses 4 structures:
  - [music] - It contains the id, the title, the artist, the album and its duration
  - [AVL_music] - it contains a structure of type "song", an integer called "h" representing pitch, and two structures similar to themselves called "left" and "right".This structure is based on a data structure called the AVL tree.
  - [playlist_node] - contains an attribute of type "music" and a structure of the same type of itself called proximo. This structure is a linked list that represents a playlist.
  - [lplaylist_no] - contains an id, a name, an attribute which has the same type called "next", quantity and the "songs" which is an attribute of the type "node_playlist". This structure is a simple linked list, it is responsible for showing all playlists.
  - During the code there are several comments to help the understanding of the program.

## Developers:

Ailton Alves
Gabriel Borges
Lygia Bueno
Renata Persch
