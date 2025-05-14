/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/14 17:39:13 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	stock_all_heredoc(t_shell *shell)
{
	t_token		*current;
	t_heredoc	*tmp;
	int			i;

	current = shell->token;
	tmp = shell->heredoc;
	while (tmp)
	{
		i = 0;
		tmp->eof_heredoc = malloc(sizeof(char *)
				* tmp->nb_heredoc);
		while (current->next)
		{
			if (current->type == END)
				tmp->eof_heredoc[i++] = current->str;
			if (current->type == END && current->next->type != HEREDOC)
			{
				current = current->next;
				break ;
			}
			current = current->next;
		}
		tmp = tmp->next;
	}
}

void	initiate_heredoc(t_shell *shell)
{
	t_token		*current;
	t_heredoc	*tmp;
	int			process;

	process = 0;
	current = shell->token;
	shell->heredoc = malloc(sizeof(t_heredoc));
	if (!shell->heredoc)
		return ;
	ft_memset(shell->heredoc, 0, sizeof(t_heredoc));
	shell->heredoc->next = NULL;
	tmp = shell->heredoc;
	while (current)
	{
		check_current_type(current, tmp, process);
		if (current->type == PIPE && (current->next->type == REDIR_IN
				|| current->next->type == HEREDOC))
		{
			check_and_create(shell, current->next, tmp);
			if (tmp->next)
				tmp = tmp->next;
			process++;
		}
		current = current->next;
	}
}

static void close_all_heredoc_fds(t_heredoc *heredoc_list)
{
    t_heredoc *current_hd;

    current_hd = heredoc_list;
    while (current_hd)
    {
        if (current_hd->p_fd[0] > 0 && current_hd->p_fd[0] != -1)
        {
            close(current_hd->p_fd[0]);
            current_hd->p_fd[0] = -1; // Marquer comme fermé
        }
        if (current_hd->p_fd[1] > 0 && current_hd->p_fd[1] != -1)
        {
            close(current_hd->p_fd[1]);
            current_hd->p_fd[1] = -1; // Marquer comme fermé
        }
        current_hd = current_hd->next;
    }
}

// Crée les pipes pour tous les heredocs actifs.
// Met à jour g_exit_status et nettoie les pipes en cas d'erreur.
// Retourne 0 en cas de succès, 1 en cas d'échec de pipe().
int create_heredoc_pipes_v2(t_shell *shell)
{
    t_heredoc *current_hd_node;

    current_hd_node = shell->heredoc;
    while (current_hd_node)
    {
        current_hd_node->p_fd[0] = -1; // Initialiser comme non ouverts/invalides
        current_hd_node->p_fd[1] = -1;
        if (current_hd_node->hrd == true) // Seulement si le heredoc est actif
        {
            if (pipe(current_hd_node->p_fd) == -1)
            {
                perror("minishell: pipe for heredoc");
                g_exit_status = 1;
                close_all_heredoc_fds(shell->heredoc); // Nettoyer les pipes déjà ouverts
                return (1);        // Échec
            }
        }
        current_hd_node = current_hd_node->next;
    }
    return (0); // Succès
}

// Gère la logique principale pour tous les heredocs.
// Appelé par l'orchestrateur avant l'exécution des commandes.
// Retourne 0 si l'exécution de la commande peut continuer (heredocs lus ou EOF).
// Retourne une valeur non nulle si l'exécution doit être annulée (erreur, SIGINT).
// g_exit_status est mis à jour en conséquence.
int handle_all_heredocs_globally_v2(t_shell *shell)
{
    int process_status;

    // initiate_heredoc_list_v2 et stock_heredoc_delimiters_v2 sont appelées
    // par l'orchestrateur avant cette fonction.
    if (!shell->heredoc) // Si initiate_heredoc n'a trouvé aucun heredoc
        return (0);      // Rien à faire, continuer.

    if (create_heredoc_pipes_v2(shell) != 0)
    {
        // g_exit_status est 1. Pipes nettoyés par create_heredoc_pipes_v2.
        ft_free_heredoc(shell); // Libérer les structures heredoc
        shell->heredoc = NULL;  // Marquer la liste comme vide
        return (g_exit_status); // Retourne 1, annuler l'exécution
    }

    process_status = process_heredoc_inputs_loop_v2(shell);

    if (process_status == 0 || process_status == 1) // Succès ou EOF global
    {
        // Pour succès (0), g_exit_status est 0 (ou dernier statut si pertinent).
        // Pour EOF (1), g_exit_status est 0 (par process_heredoc_inputs_loop_v2).
        // Dans les deux cas, l'exécution de la commande doit continuer.
        // Les pipes (p_fd[0]) sont prêts pour la lecture par les commandes.
        // Le nettoyage final (ft_free_heredoc, fermeture des p_fd[0]) se fera après l'exécution.
        return (0);
    }
    else // Interruption (SIGINT, status 2) ou Erreur interne (status 3)
    {
        // Pour SIGINT (status 2), g_exit_status est 130.
        // Pour Erreur interne (status 3), g_exit_status est 1.
        close_all_heredoc_fds(shell->heredoc); // Fermer tous les fds (lecture et écriture)
        ft_free_heredoc(shell);                // Libérer les structures heredoc
        shell->heredoc = NULL;
        if (process_status == 2) // SIGINT
            return (130);        // Annuler l'exécution, statut final 130
        else // process_status == 3, Erreur interne
            return (1);          // Annuler l'exécution, statut final 1
    }
}
