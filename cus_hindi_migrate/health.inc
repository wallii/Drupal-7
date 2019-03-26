<?php
/**
 * The ZeeNews Health Migration
 */



/**
 * The Entertainment Gourmet CategoryMigration
 */
class ZHSections extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');


        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'sec_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );


        $query = Database::getConnection('zeehealth161', 'for_migration')
            ->select('Health_section', 'cat');
        $query->fields('cat', array('sec_id', 'sec_name'));
        //    ->condition('zsm_section_id', 'STT', '=');
        // This sort assures that parents are saved before children.
        //   ->orderBy('style_parent', 'ASC');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('health_sections');
        // print $this->getQualifiedMapTable();
        // Assign mappings TO destination fields FROM source fields. To discover
        // the names used in these calls, use the drush commands
        // drush migrate-fields-destination BeerTerm
        // drush migrate-fields-source BeerTerm
        $this->addFieldMapping('name', 'sec_name');
      //  $this->addFieldMapping('description', 'section_id');
//        $this->addFieldMapping('pathauto')->defaultValue("1");
 //       $this->addFieldMapping('path','url');

//        module_load_include('inc','pathauto','pathauto');
    }
    public function prepareRow($row) {

        $row->sec_name = strip_tags(utf8_encode($row->sec_name));
    }
}


/**
 * The ZeeNews Entertainemnt Node Migration
 */

class ZHNode extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'newsid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeehealth161', 'for_migration')
            ->select('news_master', 'znews');
        $query->fields('znews', array('newsid','title','sec_id','image_courtesy','tag','author_id','nsource','news_content','znm_news_summary','EntryDate','LastUpdate','url','home_img','imgdir'));
       // $query->orderBy('newsid', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('health_news', array('text_format' => 'full_html'));
     //   $this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'title');
        $this->addFieldMapping('field_author', 'author_id')
            ->sourceMigration('ZNAuthor')
            ->arguments(array('source_type' => 'uid'));
        $this->addFieldMapping('field_healthsection', 'sec_id')
            ->sourceMigration('ZHSections')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_tags', 'tag');
        $this->addFieldMapping('field_news_source', 'nsource')
            ->sourceMigration(array('ZNSource'))
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_home_title', 'title');
        $this->addFieldMapping('field_image_caption', 'image_courtesy');
        $this->addFieldMapping('field_oldid', 'newsid');
        $this->addFieldMapping('body', 'news_content');
        $this->addFieldMapping('body:summary', 'znm_news_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'EntryDate');
        $this->addFieldMapping('changed', 'LastUpdate');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');

        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'home_img');
        $this->addFieldmapping('field_image:destination_dir','imgdir');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znn.india.com/');
    }

    public function prepareRow($row) {

        $row->title = clean_my_string($row->title);
        $row->title = utf8_encode(strip_tags($row->title));
        $row->znm_news_summary = clean_my_string($row->znm_news_summary);
        $row->znm_news_summary = utf8_encode(strip_tags($row->znm_news_summary));
        $row->image_courtsey = utf8_encode(strip_tags($row->image_courtsey));

        $row->news_content = clean_my_string($row->news_content);
        $row->news_content =  str_replace("<br>", "\n", $row->news_content);
        $row->news_content =  str_replace("<br/>", "\n", $row->news_content);
        $row->news_content =  str_replace("\n\n", "\n<br/>", $row->news_content);

        $row->news_content =  utf8_encode($row->news_content);

        if ($row->home_img == 'spacer.gif')
        {
            $row->home_img = null;
            $row->image_courtesy = null;}

        if ($row->image_courtesy = '-')
        {
            $row->image_courtesy = null;
        }

        $vFile = pathinfo($row->imgdir);
        $row->imgdir = "public://" .strtolower($vFile['dirname']);


    }
    public function prepare($node, stdClass $row) {

       // $node->nid=1000000 + $node->nid;

        $keywords = explode(',', $row->tag);
        static $vid;
        if (!isset($vid)) {
            $vocabs = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'tags'));
            $vids = array_keys($vocabs);
            $vid = reset($vids);
        }
        foreach ($keywords as $index => $keyword) {

            $keyword=trim($keyword);
            if (!empty($keyword) and strlen($keyword) > 3)
            {
                $matches = taxonomy_get_term_by_name($keyword);
                if (empty($matches)) {
                    $term = new stdClass;
                    $term->name = utf8_encode($keyword);
                    $term->vocabulary_machine_name = 'tags';
                    $term->vid = $vid;
                    taxonomy_term_save($term);
                }
                else {
                    $term = reset($matches);
                }
                $node->field_tags[LANGUAGE_NONE][$index]['tid'] = $term->tid;

            }
        }
        //  if ($node->nid < 146337)
        {
            $html = str_get_html($node->body[LANGUAGE_NONE][0]['value']);

            $total_img = count($html->find("img"));
            if ($total_img > 0) {
                for ($i = 0; $i < $total_img; $i++) {
                    $element=$html->find("img", $i);
                        $element->outertext ='';
                }
                $node->body[LANGUAGE_NONE][0]['value'] = $html->save();
                //$node->body[LANGUAGE_NONE][0]['value']=str_replace('<img','<img class="imglft" ',$node->body[LANGUAGE_NONE][0]['value']);
            }

        }
    }

}

/**
 * The Ayurveda CategoryMigration
 */
class ZASections extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');


        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'sec_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );


        $query = Database::getConnection('zeeayur161', 'for_migration')
            ->select('Ayurveda_section', 'cat');
        $query->fields('cat', array('sec_id', 'sec_name','meta_title','meta_key','meta_desc'));
        //    ->condition('zsm_section_id', 'STT', '=');
        // This sort assures that parents are saved before children.
        //   ->orderBy('style_parent', 'ASC');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('ayurveda_sections');
        // print $this->getQualifiedMapTable();
        // Assign mappings TO destination fields FROM source fields. To discover
        // the names used in these calls, use the drush commands
        // drush migrate-fields-destination BeerTerm
        // drush migrate-fields-source BeerTerm
        $this->addFieldMapping('name', 'sec_name');
        $this->addFieldMapping('metatag_title', 'meta_title');
        $this->addFieldMapping('metatag_description', 'meta_key');
        $this->addFieldMapping('metatag_keywords', 'meta_desc');

        //  $this->addFieldMapping('description', 'section_id');
//        $this->addFieldMapping('pathauto')->defaultValue("1");
        //       $this->addFieldMapping('path','url');

//        module_load_include('inc','pathauto','pathauto');
    }
    public function prepareRow($row) {

        $row->sec_name = strip_tags(utf8_encode($row->sec_name));
    }
}


/**
 * The ZeeNews Ayurveda Node Migration
 */

class ZANode extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'newsid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeeayur161', 'for_migration')
            ->select('news_master', 'znews');
        $query->fields('znews', array('newsid','title','sec_id','tag','news_content','znm_news_summary','EntryDate','url','home_img'));
        // $query->orderBy('newsid', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('ayurveda_news', array('text_format' => 'full_html'));
        //   $this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'title');
//        $this->addFieldMapping('field_author', 'author_id')
//            ->sourceMigration('ZNAuthor')
//            ->arguments(array('source_type' => 'uid'));
        $this->addFieldMapping('field_ay_section', 'sec_id')
            ->sourceMigration('ZASections')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_tags', 'tag');
        $this->addFieldMapping('field_news_source')->defaultValue(0)
            ->sourceMigration(array('ZNSource'))
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_home_title', 'title');
  //      $this->addFieldMapping('field_image_caption', 'image_courtesy');
        $this->addFieldMapping('field_oldid', 'newsid');
        $this->addFieldMapping('body', 'news_content');
        $this->addFieldMapping('body:summary', 'znm_news_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'EntryDate');
        $this->addFieldMapping('changed', 'EntryDate');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');
//        $this->addFieldMapping('field_show_home_image')->defaultValue(1);

        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'home_img');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znm.india.com/ayurveda/');
    }

    public function prepareRow($row) {


        $row->title = clean_my_string($row->title);
        $row->title = utf8_encode(strip_tags($row->title));

        $row->znm_news_summary = clean_my_string($row->znm_news_summary);
        $row->znm_news_summary = utf8_encode(strip_tags($row->znm_news_summary));
        $row->image_courtsey = utf8_encode(strip_tags($row->image_courtsey));

        $row->news_content =  clean_my_string($row->news_content);
        $row->news_content =  str_replace("<br>", "\n", $row->news_content);
        $row->news_content =  str_replace("<br/>", "\n", $row->news_content);
        $row->news_content =  str_replace("\n\n", "\n<br/>", $row->news_content);

        $row->news_content =  utf8_encode($row->news_content);
        $row->url=$row->url . '_' . $row->newsid . '.html';

        if (strlen($row->home_img) < 15)
        {
            $row->home_img = null;
}


    }
    public function prepare($node, stdClass $row) {

        // $node->nid=1000000 + $node->nid;

        $keywords = explode(',', $row->tag);
        static $vid;
        if (!isset($vid)) {
            $vocabs = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'tags'));
            $vids = array_keys($vocabs);
            $vid = reset($vids);
        }
        foreach ($keywords as $index => $keyword) {

            $keyword=trim($keyword);
            if (!empty($keyword) and strlen($keyword) > 3)
            {
                $matches = taxonomy_get_term_by_name($keyword);
                if (empty($matches)) {
                    $term = new stdClass;
                    $term->name = utf8_encode($keyword);
                    $term->vocabulary_machine_name = 'tags';
                    $term->vid = $vid;
                    taxonomy_term_save($term);
                }
                else {
                    $term = reset($matches);
                }
                $node->field_tags[LANGUAGE_NONE][$index]['tid'] = $term->tid;

            }
        }
        //  if ($node->nid < 146337)
        {
            $html = str_get_html($node->body[LANGUAGE_NONE][0]['value']);

            $total_img = count($html->find("img"));
            if ($total_img > 0) {
                for ($i = 0; $i < $total_img; $i++) {
                    $element=$html->find("img", $i);
                    $element->outertext ='';
                }
                $node->body[LANGUAGE_NONE][0]['value'] = $html->save();
                //$node->body[LANGUAGE_NONE][0]['value']=str_replace('<img','<img class="imglft" ',$node->body[LANGUAGE_NONE][0]['value']);
            }

        }
    }

}