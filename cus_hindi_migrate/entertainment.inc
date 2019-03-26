<?php
/**
 * The ZeeNews Entertainemnt Celebrity Migration
 */

class ZECelebrity extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'celeb_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('spice_celebrity_master', 'znews');
        $query->fields('znews', array('celeb_id','celeb_name','celeb_fullname','celeb_birthday','celeb_sunsign','celeb_birthplace','celeb_nationality','celeb_height','celeb_gender','celeb_domain','celeb_summary','celeb_main_image','celeb_bio','celeb_bio_image','celeb_filmography','celeb_awards','celeb_date','celeb_type'));
        $query->orderBy('celeb_id', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('celebrity', array('text_format' => 'full_html'));
        $this->addFieldMapping('nid', 'celeb_id');
        $this->addFieldMapping('title', 'celeb_name');
        $this->addFieldMapping('field_full_name', 'celeb_fullname');
        $this->addFieldMapping('field_birthday', 'celeb_birthday');
        $this->addFieldMapping('field_sun_sign', 'celeb_sunsign');
        $this->addFieldMapping('field_birth_place', 'celeb_birthplace');
        $this->addFieldMapping('field_nationality', 'celeb_nationality');
        $this->addFieldMapping('field_height', 'celeb_height');
        $this->addFieldMapping('field_gender', 'celeb_gender');
        $this->addFieldMapping('field_caption', 'celeb_filmography');
        $this->addFieldMapping('field_awards', 'celeb_awards');
        $this->addFieldMapping('field_celebrity_domain', 'celeb_domain');
        $this->addFieldMapping('field_celebrity_category', 'celeb_type');
        $this->addFieldMapping('body', 'celeb_bio');
        $this->addFieldMapping('body:summary', 'celeb_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'celeb_date');
        $this->addFieldMapping('changed', 'celeb_date');
        $this->addFieldMapping('field_oldid','celeb_id');
        $this->addFieldMapping('pathauto')->defaultValue("1");
        //$this->addFieldMapping('path','url');
//        $this->addFieldMapping('field_bio_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
       $this->addFieldmapping('field_bio_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_bio_image', 'celeb_bio_image');
        $this->addFieldMapping('field_bio_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');
//        $this->addFieldMapping('field_bio_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'celeb_main_image');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');


    }

    public function prepareRow($row) {

        $row->celeb_name = utf8_encode(strip_tags($row->celeb_name));
        $row->celeb_fullname = utf8_encode(strip_tags($row->celeb_fullname));
        $row->celeb_birthday = utf8_encode(strip_tags($row->celeb_birthday));
        $row->celeb_birthplace = utf8_encode(strip_tags($row->celeb_birthplace));
        $row->celeb_sunsign = utf8_encode(strip_tags($row->celeb_sunsign));
        $row->celeb_nationality = utf8_encode(strip_tags($row->celeb_nationality));
        $row->celeb_height = utf8_encode(strip_tags($row->celeb_height));
        $row->celeb_gender = utf8_encode(strip_tags($row->celeb_gender));
        $row->celeb_filmography = utf8_encode(($row->celeb_filmography));
        $row->celeb_awards = utf8_encode(strip_tags($row->celeb_awards));
        $row->celeb_summary = utf8_encode(strip_tags($row->celeb_summary));
        $row->celeb_bio = utf8_encode(strip_tags($row->celeb_bio));
        $row->celeb_filmography = utf8_encode(($row->celeb_filmography));


        //   $row->znm_news_title= $row->znm_news_title .'Rana';
        // $row->news_content =  str_replace("&lt;br&gt;&lt;br&gt;", "\n", $row->news_content);
        $row->celeb_awards =  str_replace("<br/>", "\n", $row->celeb_awards);
        $row->celeb_awards =  str_replace("<br>", "\n", $row->celeb_awards);

        $row->celeb_filmography =  str_replace("<br/>", "\n", $row->celeb_filmography);
        $row->celeb_filmography =  str_replace("<br>", "\n", $row->celeb_filmography);
        // $row->news_content =  str_replace("&lt;", "<", $row->news_content);


    }
    public function prepare($node, stdClass $row) {
        $node->nid=1550000 + $node->nid;
    }
}

/**
 * The Entertainment CategoryMigration
 */
class ZESections extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');


        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'section_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );


        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('spice_section_master', 'cat');
        $query->fields('cat', array('section_id', 'section_name','url'));
        //    ->condition('zsm_section_id', 'STT', '=');
        // This sort assures that parents are saved before children.
        //   ->orderBy('style_parent', 'ASC');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('ze_news_sections');
        // print $this->getQualifiedMapTable();
        // Assign mappings TO destination fields FROM source fields. To discover
        // the names used in these calls, use the drush commands
        // drush migrate-fields-destination BeerTerm
        // drush migrate-fields-source BeerTerm
        $this->addFieldMapping('name', 'section_name');
        $this->addFieldMapping('description', 'section_id');
        $this->addFieldMapping('pathauto')->defaultValue("1");
        //$this->addFieldMapping('path','url');


    }
    public function prepareRow($row) {

        $row->section_name = strip_tags($row->section_name);
       // $row->url='entertainment/'.$row->url;

    }
}


/**
 * The ZeeNews Entertainemnt Node Migration
 */

class ZENode extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'story_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('news_master', 'znews');
        $query->fields('znews', array('story_id','story_title','section_id','domain_id','image_courtesy','tag','author_id','nsource','story_review','story_rating','news_content','story_summary','story_date','published','url','story_image'));
        $query->orderBy('story_id', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('entertainment_news', array('text_format' => 'full_html'));
        $this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'story_title');
        $this->addFieldMapping('field_author', 'author_id')
            ->sourceMigration('ZNAuthor')
            ->arguments(array('source_type' => 'uid'));
        $this->addFieldMapping('field_ze_section', 'section_id')
            ->sourceMigration('ZESections')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_tags', 'tag');
        $this->addFieldMapping('field_news_source', 'nsource')
            ->sourceMigration(array('ZNSource'))
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_celebrity_domain', 'domain_id');
        $this->addFieldMapping('field_home_title', 'story_title');
        $this->addFieldMapping('field_image_caption', 'image_courtesy');
        $this->addFieldMapping('field_oldid', 'story_id');
        $this->addFieldMapping('field_is_review', 'story_review');
        $this->addFieldMapping('field_rating', 'story_rating');
        $this->addFieldMapping('body', 'news_content');
        $this->addFieldMapping('body:summary', 'story_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'story_date');
        $this->addFieldMapping('changed', 'published');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');

        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'story_image');
     //   $this->addFieldmapping('field_image:destination_dir','imgdir');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');
//        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
//        $this->addFieldMapping('field_image', 'story_image');
//        $this->addFieldMapping('field_image:source_dir')
//            ->defaultValue('http://origin.zns.india.com/upload');


    }

    public function prepareRow($row) {
        $row->story_title = clean_my_string($row->story_title);
        $row->story_title = utf8_encode(strip_tags($row->story_title));

        $row->story_summary = clean_my_string($row->story_summary);
        $row->story_summary = utf8_encode(strip_tags($row->story_summary));
        $row->image_courtsey = utf8_encode(strip_tags($row->image_courtsey));

        $row->news_content = clean_my_string($row->news_content);
        $row->news_content =  str_replace("<br>", "\n", $row->news_content);
        $row->news_content =  str_replace("<br/>", "\n", $row->news_content);
        $row->news_content =  str_replace("\n\n", "\n<br/>", $row->news_content);
        // $row->news_content =  str_replace("&lt;", "<", $row->news_content);
        $row->news_content =  utf8_encode($row->news_content);

        $row->url='entertainment/' . $row->url . '_' . $row->story_id . '.html';
        if ($row->image_courtesy = '-')
        {
            $row->image_courtesy = null;
        }
        if (strlen($row->story_image) < 12)
        {
            $row->story_image = null;
            $row->image_courtesy = null;}
    }
    public function prepare($node, stdClass $row) {

        $node->nid=1000000 + $node->nid;

        $keywords = explode(',', $row->tag);
        static $vid;
        if (!isset($vid)) {
            $vocabs = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'tags'));
            $vids = array_keys($vocabs);
            $vid = reset($vids);
        }
        foreach ($keywords as $index => $keyword) {

            $keyword=trim($keyword);
            if (!empty($keyword) and strlen($keyword)>2)
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
                    $src = $html->find("img", $i)->src;
                    //$src=str_replace('``','',$src);
                    $remoteDocPath = strtolower($src);
                    $doc = system_retrieve_file($remoteDocPath,NULL, true, FILE_EXISTS_RENAME);
                    //  $mfiles = drupal_add_existing_file($doc);
                    if ( strlen($doc->uri) > 10){
                        $html->find("img", $i)->src = str_replace('public://','/sites/default/files/', $doc->uri);
                    }
                    else{
                        $element->outertext ='';
                    }
                }

                $node->body[LANGUAGE_NONE][0]['value'] = $html->save();
                $node->body[LANGUAGE_NONE][0]['value']=str_replace('<img','<img class="imglft" ',$node->body[LANGUAGE_NONE][0]['value']);
            }
        }
    }

}

/**
 * The Entertainment Gourmet CategoryMigration
 */
class ZEGSections extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');


        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'section_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );


        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('spice_gourmet_section_master', 'cat');
        $query->fields('cat', array('section_id', 'section_name','url'));
        //    ->condition('zsm_section_id', 'STT', '=');
        // This sort assures that parents are saved before children.
        //   ->orderBy('style_parent', 'ASC');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('gourmet_sections');
        // print $this->getQualifiedMapTable();
        // Assign mappings TO destination fields FROM source fields. To discover
        // the names used in these calls, use the drush commands
        // drush migrate-fields-destination BeerTerm
        // drush migrate-fields-source BeerTerm
        $this->addFieldMapping('name', 'section_name');
        $this->addFieldMapping('description', 'section_id');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');

        module_load_include('inc','pathauto','pathauto');
    }
    public function prepareRow($row) {

        $row->section_name = strip_tags($row->section_name);
        $row->url='entertainment/gourmet/'. pathauto_cleanstring($row->section_name).'-recipes'.$row->section_id.'.htm';
    }
}


/**
 * The Entertainment Gourmet CategoryMigration
 */
class ZEGSubSec extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');


        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'cat_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );


        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('spice_gourmet_category_master', 'cat');
        $query->fields('cat', array('cat_id', 'cat_title','cat_desc'));
        //    ->condition('zsm_section_id', 'STT', '=');
        // This sort assures that parents are saved before children.
        //   ->orderBy('style_parent', 'ASC');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('gourmet_category');
        // print $this->getQualifiedMapTable();
        // Assign mappings TO destination fields FROM source fields. To discover
        // the names used in these calls, use the drush commands
        // drush migrate-fields-destination BeerTerm
        // drush migrate-fields-source BeerTerm
        $this->addFieldMapping('name', 'cat_title');
        $this->addFieldMapping('description', 'cat_id');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','cat_desc');

        module_load_include('inc','pathauto','pathauto');
    }
    public function prepareRow($row) {

        $row->cat_title = strip_tags($row->cat_title);
        $row->cat_desc='entertainment/gourmet/'. pathauto_cleanstring($row->cat_title).'-recipes-sec'.$row->cat_id.'.htm';
    }
}


/**
 * The ZeeNews Entertainemnt Node Migration
 */

class ZEGNode extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'story_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('news_gmaster', 'znews');
        $query->fields('znews', array('story_id','story_title','image_courtesy','nsource','tag','news_content','story_summary','story_date','edit_date','url','story_image','section','subsec','author_id','story_rec_image','imgdir'));
        $query->orderBy('story_id', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('recipe', array('text_format' => 'full_html'));
       // $this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'story_title');
        $this->addFieldMapping('field_author', 'author_id')
            ->sourceMigration('ZNAuthor')
            ->arguments(array('source_type' => 'uid'));
        $this->addFieldMapping('field_recipe_sections', 'section')
            ->separator(',')
            ->sourceMigration('ZEGSections')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_recipe_subsection', 'subsec')
            ->separator(',')
            ->sourceMigration('ZEGSubSec')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_tags', 'tag');
        $this->addFieldMapping('field_news_source', 'nsource')
            ->sourceMigration(array('ZNSource'))
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_home_title', 'story_title');
        $this->addFieldMapping('field_image_caption', 'image_courtesy');
        $this->addFieldMapping('field_oldid', 'story_id');
        $this->addFieldMapping('body', 'news_content');
        $this->addFieldMapping('body:summary', 'story_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'story_date');
        $this->addFieldMapping('changed', 'edit_date');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');
        $this->addFieldMapping('field_recipe_type','story_rec_image');
        $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'story_image');
        $this->addFieldmapping('field_image:destination_dir','imgdir');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');
    }

    public function prepareRow($row) {
        $vFile = pathinfo($row->imgdir);
        $row->imgdir = "public://" .strtolower($vFile['dirname']);

        $row->story_title = clean_my_string($row->story_title);
        $row->story_title = utf8_encode(strip_tags($row->story_title));
        $row->story_summary = clean_my_string($row->story_summary);
        $row->story_summary = utf8_encode(strip_tags($row->story_summary));
        $row->image_courtsey = utf8_encode(strip_tags($row->image_courtsey));
        $row->news_content = clean_my_string($row->news_content);
        $row->news_content =  str_replace("<br>", "\n", $row->news_content);
        $row->news_content =  str_replace("<br/>", "\n", $row->news_content);
        $row->news_content =  str_replace("\n\n", "\n", $row->news_content);
         $row->news_content =  utf8_encode($row->news_content);

        $row->url='entertainment/gourmet/recipe/' . $row->url . '_' . $row->story_id . '.htm';

        if ($row->image_courtesy == '-'){
            $row->image_courtesy = null;
        }
        if ($row->author_id == 0)
        {
            $row->author_id = null;
        }
        if ($row->story_rec_image == 'images/spacer.gif' or $row->story_rec_image == 'images/grn-bult-vg.jpg'  or $row->story_rec_image == '/images/grn-bult-vg.jpg' )
        {
            $row->story_rec_image = 1;
        }
        else
        {
            $row->story_rec_image = 2;

        }


    }
    public function prepare($node, stdClass $row) {

 //       $node->nid=1000000 + $node->nid;

        $keywords = explode(',', $row->tag);
        static $vid;
        if (!isset($vid)) {
            $vocabs = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'tags'));
            $vids = array_keys($vocabs);
            $vid = reset($vids);
        }
        foreach ($keywords as $index => $keyword) {

            $keyword=trim($keyword);
            if (!empty($keyword) and strlen($keyword)>2)
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
//          if ($node->nid < 146337)
//        {
//            $html = str_get_html($node->body[LANGUAGE_NONE][0]['value']);
//
//            $total_img = count($html->find("img"));
//            if ($total_img > 0) {
//                for ($i = 0; $i < $total_img; $i++)  {
//                    $element=$html->find("img", $i);
//                    $src = $html->find("img", $i)->src;
//
//                    $remoteDocPath = strtolower($src);
//                    $doc = system_retrieve_file($remoteDocPath,NULL, true, FILE_EXISTS_RENAME);
//                    //  $mfiles = drupal_add_existing_file($doc);
//                    if ( strlen($doc->uri) > 10){
//                        $html->find("img", $i)->src = str_replace('public://','/sites/default/files/', $doc->uri);
//                    }
//                    else{
//                        $element->outertext ='';
//                    }
//                }
//
//                $node->body[LANGUAGE_NONE][0]['value'] = $html->save();
//                $node->body[LANGUAGE_NONE][0]['value']=str_replace('<img','<img class="imglft" ',$node->body[LANGUAGE_NONE][0]['value']);
//            }
//
//        }
    }

}
/**
 * The Entertainment Travel CategoryMigration
 */
class ZETSections extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');


        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'section_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );


        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('spice_travel_section', 'cat');
        $query->fields('cat', array('section_id', 'section_name'));
        //    ->condition('zsm_section_id', 'STT', '=');
        // This sort assures that parents are saved before children.
        //   ->orderBy('style_parent', 'ASC');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('travel_sections');
        // print $this->getQualifiedMapTable();
        // Assign mappings TO destination fields FROM source fields. To discover
        // the names used in these calls, use the drush commands
        // drush migrate-fields-destination BeerTerm
        // drush migrate-fields-source BeerTerm
        $this->addFieldMapping('name', 'section_name');
        $this->addFieldMapping('description', 'section_id');
        $this->addFieldMapping('pathauto')->defaultValue("1");
        //$this->addFieldMapping('path','url');


    }
    public function prepareRow($row) {

        $row->section_name = strip_tags($row->section_name);
        // $row->url='entertainment/'.$row->url;

    }
}


/**
 * The ZeeNews Entertainemnt Travel Node Migration
 */

class ZETNode extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'story_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('new_tmaster', 'znews');
        $query->fields('znews', array('story_id','story_title','section_id','domain_id','image_courtesy','tag','author_id','nsource','news_content','story_summary','story_date','published','url','story_image'));
        $query->orderBy('story_id', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('travel_news', array('text_format' => 'full_html'));
     //   $this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'story_title');
        $this->addFieldMapping('field_author', 'author_id')
            ->sourceMigration('ZNAuthor')
            ->arguments(array('source_type' => 'uid'));
        $this->addFieldMapping('field_tr_section', 'domain_id')
            ->sourceMigration('ZETSections')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_tags', 'tag');
        $this->addFieldMapping('field_news_source', 'nsource')
            ->sourceMigration(array('ZNSource'))
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_home_title', 'story_title');
        $this->addFieldMapping('field_image_caption', 'image_courtesy');
        $this->addFieldMapping('field_oldid', 'story_id');
        $this->addFieldMapping('field_is_review', 'section_id');
        $this->addFieldMapping('body', 'news_content');
        $this->addFieldMapping('body:summary', 'story_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'story_date');
        $this->addFieldMapping('changed', 'published');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');

        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'story_image');
        //   $this->addFieldmapping('field_image:destination_dir','imgdir');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');
//        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
//        $this->addFieldMapping('field_image', 'story_image');
//        $this->addFieldMapping('field_image:source_dir')
//            ->defaultValue('http://origin.zns.india.com/upload');


    }

    public function prepareRow($row) {
        if ($row->domain_id > 102)
        {
            $row->section_id = 1;
        }
        $row->story_title = clean_my_string($row->story_title);
        $row->story_title = utf8_encode(strip_tags($row->story_title));

        $row->story_summary = clean_my_string($row->story_summary);
        $row->story_summary = utf8_encode(strip_tags($row->story_summary));
        $row->image_courtsey = utf8_encode(strip_tags($row->image_courtsey));

        $row->news_content = clean_my_string($row->news_content);
        $row->news_content =  str_replace("<br>", "\n", $row->news_content);
        $row->news_content =  str_replace("<br/>", "\n", $row->news_content);
        $row->news_content =  str_replace("\n\n", "\n<br/>", $row->news_content);
        // $row->news_content =  str_replace("&lt;", "<", $row->news_content);
        $row->news_content =  utf8_encode($row->news_content);

        $row->url='entertainment/travel/' . $row->url . '_' . $row->story_id . '.htm';
        if ($row->image_courtesy = '-')
        {
            $row->image_courtesy = null;
        }
        if (strlen($row->story_image) < 12)
        {
            $row->story_image = null;
            $row->image_courtesy = null;}
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
            if (!empty($keyword) and strlen($keyword)>2)
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
                    $src = $html->find("img", $i)->src;
                    //$src=str_replace('``','',$src);
                    $remoteDocPath = strtolower($src);
                    $doc = system_retrieve_file($remoteDocPath,NULL, true, FILE_EXISTS_RENAME);
                    //  $mfiles = drupal_add_existing_file($doc);
                    if ( strlen($doc->uri) > 10){
                        $html->find("img", $i)->src = str_replace('public://','/sites/default/files/', $doc->uri);
                    }
                    else{
                        $element->outertext ='';
                    }
                }

                $node->body[LANGUAGE_NONE][0]['value'] = $html->save();
                $node->body[LANGUAGE_NONE][0]['value']=str_replace('<img','<img class="imglft" ',$node->body[LANGUAGE_NONE][0]['value']);
            }
        }
    }

}


/**
 * The Entertainment Bookworm CategoryMigration
 */
class ZEBSections extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');


        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'sid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );


        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('spice_Bookwormsection_master', 'cat');
        $query->fields('cat', array('sid', 'section'));
        //    ->condition('zsm_section_id', 'STT', '=');
        // This sort assures that parents are saved before children.
        //   ->orderBy('style_parent', 'ASC');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('bookworm_sections');
        // print $this->getQualifiedMapTable();
        // Assign mappings TO destination fields FROM source fields. To discover
        // the names used in these calls, use the drush commands
        // drush migrate-fields-destination BeerTerm
        // drush migrate-fields-source BeerTerm
        $this->addFieldMapping('name', 'section');
        $this->addFieldMapping('description', 'sid');
       // $this->addFieldMapping('pathauto')->defaultValue("1");
        //$this->addFieldMapping('path','url');


    }
    public function prepareRow($row) {

        $row->section_name = strip_tags($row->section_name);
        // $row->url='entertainment/'.$row->url;

    }
}



/**
 * The ZeeNews Entertainemnt Bookworm Node Migration
 */

class ZEBNode extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'nid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('news_bmaster', 'znews');
        $query->fields('znews', array('nid','story_title','secid','image_courtesy','tags','nsource','news_content','story_desc','entry_date','url','story_image'));
        $query->orderBy('nid', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('bookworm', array('text_format' => 'full_html'));
        //   $this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'story_title');
        $this->addFieldMapping('field_bw_section', 'secid')
            ->sourceMigration('ZEBSections')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_tags', 'tags');
        $this->addFieldMapping('field_news_source', 'nsource')
            ->sourceMigration(array('ZNSource'))
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_home_title', 'story_title');
        $this->addFieldMapping('field_image_caption', 'image_courtesy');
        $this->addFieldMapping('field_oldid', 'nid');
        $this->addFieldMapping('body', 'news_content');
        $this->addFieldMapping('body:summary', 'story_desc');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'entry_date');
        $this->addFieldMapping('changed', 'entry_date');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');

        $this->addFieldMapping('field_image', 'story_image');
        //   $this->addFieldmapping('field_image:destination_dir','imgdir');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');
//        $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
//        $this->addFieldMapping('field_image', 'story_image');
//        $this->addFieldMapping('field_image:source_dir')
//            ->defaultValue('http://origin.zns.india.com/upload');


    }

    public function prepareRow($row) {

        $row->story_title = clean_my_string($row->story_title);
        $row->story_title = utf8_encode(strip_tags($row->story_title));

        $row->story_desc = clean_my_string($row->story_desc);
        $row->story_desc = utf8_encode(strip_tags($row->story_desc));
        $row->image_courtsey = utf8_encode(strip_tags($row->image_courtsey));

        $row->news_content = clean_my_string($row->news_content);
        $row->news_content =  str_replace("<br>", "\n", $row->news_content);
        $row->news_content =  str_replace("<br/>", "\n", $row->news_content);
        $row->news_content =  str_replace("\n\n", "\n<br/>", $row->news_content);
        // $row->news_content =  str_replace("&lt;", "<", $row->news_content);
        $row->news_content =  utf8_encode($row->news_content);

        $row->url='entertainment/bookworm/' . $row->url . '_' . $row->nid . '.htm';
        if ($row->image_courtesy = '-')
        {
            $row->image_courtesy = null;
        }
        if (stripos('spacer.gif',$row->story_image) == false)
        {}
        else
        {
            $row->story_image = null;
            $row->image_courtesy = null;}

        if ($row->image_courtesy = '-')
        {
            $row->image_courtesy = null;
        }
        if ($row->nid <3104){$row->story_image = null;}
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
            if (!empty($keyword) and strlen($keyword)>2)
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

    }

}



/**
 * The Entertainment Video Migration
 */
class ZNEVideo extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        //   $this->dependencies = array('ZeeNewsVideoCategory');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'video_id' => array(
                    'type' => 'int',
                    'not null' => false,
                    'alias' => 'zvids',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('news_videos', 'zvids'); //SELECT     TOP (100) PERCENT iVediouplaod, vFilename, dUploaddate, LTRIM(RTRIM(cSection)) AS csection, cDescription, cTitle, dbo.RemoveSpecialChars(cTitle) AS url         FROM         dbo.zn_vedio_upload  WHERE     (isYoutube = 0) ORDER BY iVediouplaod
        $query->fields('zvids', array('video_id','video_title','video_desc','video_file','video_image','youtubecode', 'isYoutube','video_date','url'));

        $query->orderBy('video_id', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('videos', array('text_format' => 'full_html'));
        //$this->addFieldMapping('nid', 'ivediouplaod');
        $this->addFieldMapping('field_oldid', 'video_id');
        $this->addFieldMapping('body', 'video_desc');
        $this->addFieldMapping('title', 'video_title');
        $this->addFieldMapping('field_yt_code', 'youtubecode');
        $this->addFieldMapping('field_home_title', 'video_title');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        // $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'video_date');
        $this->addFieldMapping('changed', 'video_date');
        $this->addFieldMapping('field_domain')->defaultValue("2");
        $this->addFieldMapping('field_isyoutube','isYoutube');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');

        //   $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        // $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'video_image');
        // $this->addFieldMapping('field_image:source_dir')
        //    ->defaultValue('');

        //  $this->addFieldMapping('field_video_file:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        $this->addFieldmapping('field_video_file:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_video_file', 'video_file');
        $this->addFieldMapping('field_video_file:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');

    }

    public function prepareRow($row) {
        if ($row->isYoutube == 1)
        {
            $row->video_file = null;
        }
        else
        {
            $row->youtubecode = null;
        }

        if (strpos($row->video_image,'http') !== false)
        {
            //print $row->img_path;
        }
        else
        {
            $row->video_image = 'http://origin.zns.india.com/upload' . $row->video_image;

        }


        $row->video_title =  ((strip_tags(trim($row->video_title))));
        $row->video_desc = ((strip_tags(trim($row->video_desc))));



        $row->url='entertainment/' .$row->url;
    }
    public function prepare($node, stdClass $row) {
        // $node->nid=1505000 + $node->nid;

        // Let's pretend we couldn't have done this as easily in prepareRow...
        //  $node->path = 'news/' . $row->znm_news_id . '.html';
    }

}


/**
 * The Zee News Entertainment Gallery Migration
 */
class ZEGallery extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
//        $this->dependencies = array('ZeeNewsCategory');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'photo_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('news_gallery', 'znews');
        $query->fields('znews', array('photo_id','photo_title','url','photo_date','photo_image'));
//        $query->condition('photo_id', '153544', '<=');
        $query->orderBy('photo_id', 'asc');//$query->range(0, 5);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('photo_gallery', array('text_format' => 'full_html'));
        // $this->addFieldMapping('nid', 'secid');
        $this->addFieldMapping('field_oldid', 'photo_id');
        $this->addFieldMapping('title', 'photo_title');
        $this->addFieldMapping('field_home_title', 'photo_title');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'photo_date');
        $this->addFieldMapping('changed', 'photo_date');
        $this->addFieldMapping('field_domain')->defaultValue("2");
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');
        $this->addFieldMapping('field_image', 'photo_image');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');


        //$this->addFieldMapping('path','znm_news_stitle');



        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
//
//        if (module_exists('path')) {
//            $this->addFieldMapping('path')
//                ->issueGroup(t('DNM'));
//            if (module_exists('pathauto')) {
//                $this->addFieldMapping('pathauto')
//                    ->issueGroup(t('DNM'));
//            }
//        }
    }

    public function prepareRow($row) {

        $row->url='entertainment/gallery'.$row->url;
        //   $row->secid=1500000 + $row->secid;

        //   $row->znm_news_title= $row->znm_news_title .'Rana';

    }

}


/**
 * The NodeImagesMigration - Field Collection
 */
class ZNEGalleryImages extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $this->dependencies = array('ZEGallery');
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'image_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationFieldCollection::getKeySchema()
        );


        $query = Database::getConnection('zeespice101', 'for_migration')
            ->select('Spice_Photogallery_Transaction', 'znews');
        $query->fields('znews', array('image_id','image_title','image_file','photo_id'));
        //   $query->condition('secid', '811', '<');
     //   $query->condition('photo_id', '153544', '<=');
//        $query->condition('photo_id', '3990', '<');

        $query->orderBy('image_id', 'asc');
//        $query->range(0, 200);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationFieldCollection('field_gallery_data',array('host_entity_type' => 'node'));

        $this->addFieldMapping('host_entity_id', 'photo_id')
            ->sourceMigration('ZEGallery');
        //    $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        //    $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'image_file');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');
        $this->addFieldMapping('field_caption', 'image_title');
        //   $this->addFieldMapping('pathauto')->defaultValue("0");
        //$this->addFieldMapping('field_caption')->defaultValue('Ultra Orthodox Jewish men wrapped in prayer shawls participate in the special "Blessing of the Sun" prayer at historic site of Masada overlooking the Dead Sea, in Masada, Israel, Wednesday. Devout Jews around the world on Wednesday observed a ritual perfo sdsds jsdhjsdhj 123');
        // ... Add field mapping here.


        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled

    }

    public function prepareRow($row) {
    }
}



/**
 * The Zee News Sports Slideshow Migration
 */
class ZESlideshow extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
//        $this->dependencies = array('ZeeNewsCategory');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'sid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeesports101', 'for_migration')
            ->select('sports_slide_section', 'znews');
        $query->fields('znews', array('sid','sec_name','sec_img','entry_date','status'));
        $query->orderBy('sid', 'asc');//$query->range(0, 5);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('slideshow', array('text_format' => 'full_html'));
        // $this->addFieldMapping('nid', 'secid');
        $this->addFieldMapping('field_oldid', 'sid');
        $this->addFieldMapping('title', 'secname');
        $this->addFieldMapping('field_home_title', 'sec_name');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'entry_date');
        $this->addFieldMapping('changed', 'entry_date');
        $this->addFieldMapping('field_domain')->defaultValue("3");
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','status');
        $this->addFieldMapping('field_image', 'sec_img');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znn.india.com/');

        //$this->addFieldMapping('path','znm_news_stitle');



        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
//
//        if (module_exists('path')) {
//            $this->addFieldMapping('path')
//                ->issueGroup(t('DNM'));
//            if (module_exists('pathauto')) {
//                $this->addFieldMapping('pathauto')
//                    ->issueGroup(t('DNM'));
//            }
//        }
//        module_load_include('inc','pathauto','pathauto');
    }

    public function prepareRow($row) {
        $row->status='entertainment/slideshow/'. pathauto_cleanstring($row->sec_name).'_'.$row->sid.'.html';
    }

}



/**
 * The NodeImagesMigration - Field Collection
 */
class ZESlideImages extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $this->dependencies = array('ZESlideshow');
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'nid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationFieldCollection::getKeySchema()
        );

        $query = "SELECT nid,sid,story_image,story_title,story_desc,story_by FROM spice_slidenews_master";
        $count_query = "SELECT COUNT(*) FROM spice_slidenews_master";
//        $query = "SELECT story_id,story_title,sections,domain_id,sections,image_courtesy FROM news_master";
        $fields = array(
            'nid' => t('story_id'),
            'sid' => t('story_title ID'),
            'story_image' => t('Story Image'),
            'story_title' => t('Story Title'),
            'story_desc' => t('story desc'),
            'story_by' => t('story By'),
        );
        $this->source = new MigrateSourceMSSQL(array(
            'servername' => '180.179.93.213',
            'username' => 'sa',
            'password' => 'P@ssw0rd',
            'database' => 'ZeeSpice',
        ), $query, $count_query, $fields);

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationFieldCollection('field_gallery_data',array('host_entity_type' => 'node'));

        $this->addFieldMapping('host_entity_id', 'sid')
            ->sourceMigration('ZESlideshow');
        //   $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        //    $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'story_image');
        $this->addFieldMapping('field_caption', 'story_title');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.zns.india.com/upload');

        //   $this->addFieldMapping('pathauto')->defaultValue("0");
        //$this->addFieldMapping('field_caption')->defaultValue('Ultra Orthodox Jewish men wrapped in prayer shawls participate in the special "Blessing of the Sun" prayer at historic site of Masada overlooking the Dead Sea, in Masada, Israel, Wednesday. Devout Jews around the world on Wednesday observed a ritual perfo sdsds jsdhjsdhj 123');
        // ... Add field mapping here.


        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled

    }

    public function prepareRow($row) {
        $row->story_title = '<h3>'.$row->story_title .'</h3><br/>' . $row->story_desc   .'<br/> - ' . $row->story_by;

    }
}
