Islandora Meme Solution Pack
============================
This dummy module gives was created for Islandora Camp Colorado in order to give you a framework to learn how to query solr in Islandora.

Bootstrapping your VM
---------------------
What good is a tutorial on querying solr if there’s no content to query?  Follow these steps to give your Islandora Camp Colorado VM enough data to make this meaningful.

From the admin toolbar, go to Islandora -> Solution pack configuration -> Solution pack required objects.  Scroll down to ‘Islandora Meme’ and select all the checkboxes.  Then click the ‘Force reinstall objects’ button.  This will install all the collections required for this module.

If you’re curious, feel free to check out the source code in the .module file to see how we’re creating these collecitons in the islandora_required_objects hook.

Next, download the .zip files from the ‘content’ folder of this module from github.  Alternatively, if you’re savvy with the command line, you can scp them over like so:

`scp -P 2222 vagrant@localhost:/var/www/drupal/htdocs/sites/all/modules/islandora_solution_pack_meme/content/*.zip .`

Now let’s feed these files to the batch importer.  Navigate to `localhost:8181/islandora/object/islandora:meme_collection`.  For each collection contained therein, click on its 'manage' tab.  Then click on its ‘collection’ tab.  Then click on the ‘Batch imort objects’ link.  Select ‘ZIP File Importer’ from the dropdown, click next, and upload the corresponding .zip file.  Make sure to click the ‘New image’ checkbox to give each object the content model from the basic image solution pack.

Be sure to do this for all three collections: Condascending Wonka, Success Kid, and First World Problems.

That’s it, you now have plenty of silly objects to work with.  Best of all, because of the gsearch setup on these VMs, all the metadata from these objects have been automatically indexed in Solr!

Let’s Query Already!
------------
Open up the islandora_solution_pack_meme.drush.inc file in your preferred text editor.  What you see before you is the boilerplate for implementing your own custom drush script.  We’re going to use it to run the following code snippets.  You run this drush command on the command line via `drush -u 1 islandora-solution-pack-meme-query-solr`.

Getting Members of a Collection
-------------------------------
```php
$qp = new IslandoraSolrQueryProcessor();
$query = 'RELS_EXT_isMemberOfCollection_uri_ms:"info:fedora/islandora:meme_collection"';
$qp->buildQuery($query);
$qp->executeQuery();

$objects = $qp->islandoraSolrResult['response']['objects'];
drush_print_r($objects);
```

Wow!  Look at all that output!  That seems like a lot, so let’s limit the fields that get returned.

Limiting returned fields
------------------------
```php
$qp = new IslandoraSolrQueryProcessor();
$query = 'RELS_EXT_isMemberOfCollection_uri_ms:"info:fedora/islandora:meme_collection"';
$qp->buildQuery($query);
$qp->solrParams['fl'] = "PID,RELS_EXT_isMemberOfCollection_uri_ms";
$qp->executeQuery();

$objects = $qp->islandoraSolrResult['response']['objects'];
drush_print_r($objects);
```

Ok, now that’s a lot more manageable.  For each result, the ‘solr_doc’ only contains what we specified.  But wait a minute, how are you magically knowing all of these cryptic field names?

Finding out what’s in your index
--------------------------------
This couldn’t be easier.  Simply pop open a web browser and navigate to Solr’s built in schema browser at http://localhost:8080/solr/#/core1/schema-browser.  This page will show you everything that’s been indexed.

But gee, those field names are pretty cryptic.  Well... yeah... they are.  But it’s not meaningless.  It really is just naming conventions.  Each field is prefixed with the datastream it comes from (e.g. MODS or RELS_EXT).  Each field is also suffixed with letters describing the type of field:  ‘m’ for multivalued, ‘s’ for string, and ‘t’ for text.  These can be concatenated, so you’ll often see ‘ms’ or ‘mt’ on the end of field names.

Actually getting some memes
---------------------------
Our output from our previous query only gave us the children of the meme_collection, which is nothing but three other collections.  That’s probably not too useful, so let’s query for something more significant.  I’ve given each meme a MODS genre of ‘Meme’.  Let’s take advantage of that.

```php
$qp = new IslandoraSolrQueryProcessor();
$query = 'mods_genre_ms:Meme';
$qp->buildQuery($query);
$qp->solrParams['fl'] = "PID,RELS_EXT_isMemberOfCollection_uri_ms";
$qp->executeQuery();

$results = $qp->islandoraSolrResult['response'];
drush_print_r($results);
```

Limiting by collection
----------------------
Use the 'fq' solr param to apply a filter query:

```php
$qp = new IslandoraSolrQueryProcessor();
$query = 'mods_genre_ms:Meme';
$qp->buildQuery($query);
$qp->solrParams['fl'] = "PID,RELS_EXT_isMemberOfCollection_uri_ms";
$qp->solrParams['fq'] = 'RELS_EXT_isMemberOfCollection_uri_ms:"info:fedora/islandora:condascending_wonka_collection"';
$qp->executeQuery();

$results = $qp->islandoraSolrResult['response'];
drush_print_r($results);
```
