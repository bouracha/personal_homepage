/*
   PARSER. Field 2 in ARGP.
   Order of parameters: KEY, ARG, STATE.
*/
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    args *arguments = state->input;

    switch (key)
    {
        case 't':
            arguments->output_interval = atoi(arg);
            break;
        case 'v':
            arguments->verbose = 1;
            break;
        case 'N':
            arguments->output_files = 0;
            break;
        case 'S':
            arguments->output_stdout = 0;
            break;
        case 'c':
            arguments->config_file = arg;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1)
            {
                argp_usage(state);
            }
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 1)
            {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
/* Description of the required arguments */
static char args_doc[] = "landscape_file";
/* Description of optional arguments
 * Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
 */
static struct argp_option options[] =
{
  {"no_output_files", 'N', 0, 0, "Run the simulation with no files written"},
  {"silent", 'S', 0, 0, "Run the simulation without displaying the average densities"},
  {"interval", 't', "T", 0, "Manually specify the output interval"},
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"config",  'c', "CONFIG_FILE", 0, "Load the parameters from CONFIG_FILE"},
  {0}
};
/* Documentation String */
static char doc[] = "population -- A program to simulate the predator-prey model of hares and pumas.\vDeveloped by:\n\tAnthony Bourached\n\tJames Clark\n\tIshita Mathur\n\tJames Si";
/*   The ARGP structure itself. */
static struct argp argp = {options, parse_opt, args_doc, doc};
