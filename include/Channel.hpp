/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 16:04:31 by ggoncalv          #+#    #+#             */
/*   Updated: 2026/07/15 14:51:07 by ggoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <string>
# include <vector>
// # include <algorithm>

/**
 * @brief Represents an IRC channel.
 * * Manages channel properties like name and topic, as well as its 
 * participants (standard members and operators).
 */
class Channel {
    private:
        std::string _name;
        std::string _topic;
        std::vector<Client*> _members;
        std::vector<Client*> _operators;
        std::vector<std::string> _invitedNicks;

        bool _inviteOnly;
        bool _topicRestricted;
        std::string _password;
        size_t _userLimit;

    public:
        Channel(const std::string& name);
        ~Channel();

        const std::string& getName() const;
        const std::string& getTopic() const;
        
        void setTopic(const std::string& topic);

        bool isMember(Client* client) const;
        void addMember(Client* client);
        bool isOperator(Client* client) const;
        void addOperator(Client* client);

        const std::vector<Client*>& getMembers() const;
        const std::vector<Client*>& getOperators() const;

        void broadcast(const std::string& message, Client* sender);

        void removeMember(Client* client);
        void removeOperator(Client* client);
        bool isEmpty() const;

        void invite(const std::string& nickname);
        bool isInvited(const std::string& nickname) const;

        bool isInviteOnly() const;
        void setInviteOnly(bool state);
        
        bool isTopicRestricted() const;
        void setTopicRestricted(bool state);
        
        bool hasPassword() const;
        const std::string& getPassword() const;
        void setPassword(const std::string& key);
        void removePassword();
        
        bool hasLimit() const;
        size_t getLimit() const;
        void setLimit(size_t limit);
        void removeLimit();
        
        size_t getMemberCount() const;
        std::string getModes() const;
    };

#endif