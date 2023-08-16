#pragma once

/**
 * Get a string variable from the environment, or def if it does not exists.
 */
char *getenvs(const char *name, const char *def);


/**
 * Get a int variable from the environment, or def if it does not exists.
 */
int getenvi(const char *name, int def);
