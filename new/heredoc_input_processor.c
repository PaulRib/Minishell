// Fichier: /home/ubuntu/heredoc_input_processor.c.mod
// Contient les modifications pour la gestion des signaux dans la lecture du heredoc.

#include "../includes/minishell.h"

// Fonction existante de l'utilisateur (supposée présente dans un autre fichier ou le .h)
// extern char *expand_variables(t_shell *shell, char *str);
// extern int check_end(int *i, t_shell *shell, t_heredoc **curr);

// Fonction existante de l'utilisateur pour afficher le message d'avertissement EOF
static void print_heredoc_eof_warning_msg_v2(const char *delimiter)
{
    ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
    if (delimiter)
        ft_putstr_fd((char *)delimiter, 2);
    else
        ft_putstr_fd("unknown", 2);
    ft_putstr_fd("`)\n", 2);
}

// Lit une ligne, la compare au délimiteur, l'écrit si besoin.
// Retourne:
//   0 : Ligne traitée, continuer la lecture pour le même délimiteur.
//   1 : Délimiteur trouvé.
//   2 : EOF (Ctrl+D) rencontré (g_exit_status sera 0).
//   3 : Interruption par SIGINT (g_exit_status sera 130).
static int read_heredoc_line(t_shell *shell, t_heredoc *hd_node, const char *delimiter)
{
    char *line_read;
    char *line_to_write;
    int eof_str_len;

    // ft_putstr_fd("> ", STDOUT_FILENO); // Prompt pour heredoc (optionnel)

    // Vérifier si SIGINT a été reçu avant la lecture (par le handler principal)
    ft_putstr_fd(">", 1);
	if (g_exit_status == 130)
        return (3);

    line_read = get_next_line(STDIN_FILENO);

    // Vérifier si SIGINT a été reçu pendant ou juste après get_next_line
    if (g_exit_status == 130)
    {
        if (line_read) 
            free(line_read);
        return (3);
    }

    if (!line_read) // EOF (Ctrl+D)
    {
        print_heredoc_eof_warning_msg_v2(delimiter);
        g_exit_status = 0; // Comportement Bash pour heredoc EOF
        return (2);
    }

    eof_str_len = ft_strlen(delimiter);
    if (ft_strncmp(line_read, delimiter, eof_str_len) == 0 &&
        line_read[eof_str_len] == '\n')
    {
        free(line_read);
        return (1); // Délimiteur trouvé
    }

    // Écrire la ligne dans le pipe si ce heredoc est actif (hd_node->hrd == true)
    // La logique originale de l'utilisateur pour l'écriture (via hrd) est respectée.
    if (hd_node->hrd == true)
    {
        line_to_write = expand_variables(shell, line_read); // Fonction de l'utilisateur
        ft_putstr_fd(line_to_write, hd_node->p_fd[1]);
        if (line_to_write != line_read) // Si expand_variables a alloué une nouvelle chaîne
            free(line_to_write);
    }
    free(line_read);
    return (0); // Ligne traitée, continuer
}

// Gère la boucle de lecture pour tous les heredocs.
// Retourne:
//   0 : Succès, tous les heredocs traités.
//   1 : EOF (Ctrl+D) global rencontré (g_exit_status = 0).
//   2 : Interruption globale par SIGINT (g_exit_status = 130).
//   3 : Erreur interne (ex: délimiteur manquant) (g_exit_status = 1).
int process_heredoc_inputs_loop_v2(t_shell *shell)
{
    t_heredoc   *current_hd_node;
    int         delim_idx; // Index du délimiteur actuel pour le nœud courant
    int         line_status;

    current_hd_node = shell->heredoc;
    delim_idx = 0;
    while (current_hd_node) // Boucle sur chaque nœud heredoc
    {
        // Si tous les délimiteurs pour le nœud actuel ont été traités
        if (delim_idx >= current_hd_node->nb_heredoc)
        {
            // check_end ferme p_fd[1] du nœud actuel, met à jour shell->exec->prev_fd,
            // avance current_hd_node au suivant, et réinitialise delim_idx (via le pointeur i*).
            if (check_end(&delim_idx, shell, &current_hd_node) == -1)
                break; // Plus de nœuds heredoc, sortie normale de la boucle
            continue;  // Recommencer la boucle avec le nouveau current_hd_node et delim_idx = 0
        }

        // S'assurer que le délimiteur est défini
        if (!current_hd_node->eof_heredoc || !current_hd_node->eof_heredoc[delim_idx])
        {
            g_exit_status = 1;
            return (3); // Erreur interne
        }
        
        // Lire et traiter une ligne pour le délimiteur actuel
        line_status = read_heredoc_line(shell, current_hd_node, 
                                      current_hd_node->eof_heredoc[delim_idx]);

        if (line_status == 1) // Délimiteur trouvé pour le délimiteur actuel
            delim_idx++;      // Passer au délimiteur suivant pour ce nœud
        else if (line_status == 2) // EOF global
            return (1); // g_exit_status est déjà 0
        else if (line_status == 3) // SIGINT global
            return (2); // g_exit_status est déjà 130
        // Si line_status == 0, continuer la lecture pour le même délimiteur
    }
    return (0); // Succès
}


