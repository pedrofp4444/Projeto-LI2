# Contributing

As a university group project, we cannot allow external contributors as of now. Our group members
should follow the guidelines set here.

 - Code should be indented with **TAB**s, with a **tab width of 2** characters;

 - The limit of columns per line is **100 columns**;

 - The bracket style to be used is the **1TBS K&R** variation (see example ahead);

 - All names should be written in **`snake_case`**;

 - Doxygen comments should be written in the **Javadoc** style.

## Example

```c
/**
 * @brief A very descriptive explanation in Javadoc style.
 */
int main(void) {
	/* Functions should have opening braces on the same line (unlike in K&R) */

	if (a_cosmic_ray_doesnt_hit_the_memory()) {
		puts("Hello, world!");
	}

	return 0;
}
```

## Including, but not limited to

As this is a quickly put together set of guidelines (after much heated debate), there are going
to be uncovered cases. Feel free to do whatever you feel like (just make sure your code isn't
_very_ ugly).
