/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 * Copyright (c) 2016-2017 metaverse core developers (see MVS-AUTHORS)
 *
 * This file is part of metaverse-explorer.
 *
 * metaverse-explorer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <metaverse/explorer/commands/hd-to-ec.hpp>

#include <metaverse/bitcoin.hpp>
#include <metaverse/explorer/define.hpp>
#include <metaverse/explorer/config/ec_private.hpp>



namespace libbitcoin {
namespace explorer {
namespace commands {

console_result hd_to_ec::invoke(std::ostream& output, std::ostream& error)
{
    // Bound parameters.
    const auto& key = get_hd_key_argument();
    const auto private_version = get_secret_version_option();
    const auto public_version = get_public_version_option();

    const auto key_version = key.version();
    if (key_version != private_version && key_version != public_version)
    {
        output << "ERROR_VERSION" << std::flush;
        return console_result::failure;
    }

    if (key.version() == private_version)
    {
        const auto prefixes = bc::wallet::hd_private::to_prefixes(
            key.version(), public_version);

        // Create the private key from hd_key and the public version.
        const auto private_key = bc::wallet::hd_private(key, prefixes);
        if (private_key)
        {
            output << encode_base16(private_key.secret()) << std::flush;
            return console_result::okay;
        }
    }
    else
    {
        // Create the public key from hd_key and the public version.
        const auto public_key = bc::wallet::hd_public(key, public_version);
        if (public_key)
        {
            output << bc::wallet::ec_public(public_key) << std::flush;
            return console_result::okay;
        }
    }

    output << "ERROR_VKEY" << std::flush;
    return console_result::failure;
}

} //namespace commands 
} //namespace explorer 
} //namespace libbitcoin 
