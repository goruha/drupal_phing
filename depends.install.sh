pear config-set auto_discover 1
pear channel-discover pear.phing.info
pear install --alldeps phing/phing-beta
pear install --alldeps "channel://pear.php.net/VersionControl_SVN-0.4.0"
pear install --alldeps "channel://pear.php.net/VersionControl_GIT-0.4.4"
pear install --alldeps PHP_CodeSniffer
pear channel-discover pear.phpmd.org
pear channel-discover pear.pdepend.org
pear install --alldeps phpmd/PHP_PMD
pear install pear.phpunit.de/phpcpd
pear channel-discover pear.pdepend.org
pear install pdepend/PHP_Depend-beta
