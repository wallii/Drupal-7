<?php
/**
 * The ZeeNews Hindi CategoryMigration
 */
class ZNMCategory extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        // $this->dependencies = array('ZeeNewsCategory');
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

        $query = Database::getConnection('zeemarathi101', 'for_migration')
            ->select('section_master', 'cat');
        $query->fields('cat', array('section_id','section_name'));
        //   $query->range(0, 1);
        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('marathi_section');
        $this->addFieldMapping('name', 'section_name');
        $this->addFieldMapping('description', 'section_id');
        //$this->addFieldMapping('parent')->defaultvalue('2433');
        //$this->addFieldMapping('parent_name')->defaultvalue('State News');
        //$this->addFieldMapping('parent_name','section_domain');
        //        ->sourceMigration('Category')
        //       ->arguments(array('source_type' => 'tid'));

        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
        if (module_exists('path')) {
            $this->addFieldMapping('path')
                ->issueGroup(t('DNM'));
            if (module_exists('pathauto')) {
                $this->addFieldMapping('pathauto')
                    ->issueGroup(t('DNM'));
            }
        }
    }
    public function prepareRow($row) {

    }
}


/**
 * The ZeeNews Hindi Node Migration
 */

class ZNMNode extends ZeeNewsMigration {
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

        //  $query = Database::getConnection('zeehindi161', 'for_migration')
        //      ->select('news_master', 'znews');
        //  $query->fields('znews', array('story_id','story_title','sections','domain_id','sections','image_courtesy','tag','author_id','nsource','news_content','story_summary','story_date','LastUpdate','url','story_image','story_etitle'));
        //  $query->orderBy('story_id', 'asc');
        //  $query->range(0, 1);

        $query = "SELECT story_id,story_title,sections,domain_id,sections,tag,author_id,news_content,story_summary,story_date,LastUpdate,url,story_image,story_etitle,ourl FROM news_master";
        $count_query = "SELECT COUNT(*) FROM news_master";
//        $query = "SELECT story_id,story_title,sections,domain_id,sections,image_courtesy FROM news_master";
        $fields = array(
            'story_id' => t('story_id'),
            'story_title' => t('story_title ID'),
            'sections' => t('sections sections'),
            'domain_id' => t('domain_id ID'),
            'sections' => t('sections ID'),
            'tag' => t('tag ID'),
            'author_id' => t('author_id ID'),
            'news_content' => t('news_content ID'),
            'story_summary' => t('story_summary ID'),
            'story_date' => t('story_date ID'),
            'LastUpdate' => t('LastUpdate ID'),
            'url' => t('url ID'),
            'story_image' => t('story_image ID'),
            'story_etitle' => t('story_etitle ID'),
            'ourl' => t('ourl ID'),
        );
        $this->source = new MigrateSourceMSSQL(array(
            'servername' => '180.179.93.213',
            'username' => 'sa',
            'password' => 'P@ssw0rd',
            'database' => 'ZeeNews_Marathi',
        ), $query, $count_query, $fields);
        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
//        $this->source = new MigrateSourceMSSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('marathi_news', array('text_format' => 'full_html'));
        //$this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'story_title');
        $this->addFieldMapping('field_author', 'author_id')
            ->sourceMigration('ZNMAuthor')
            ->arguments(array('source_type' => 'uid'));
        $this->addFieldMapping('field_ma_section', 'sections')
            ->separator(',')
            ->sourceMigration('ZNMCategory')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_tags', 'tag');
        $this->addFieldMapping('field_news_source', 'nsource')
            ->sourceMigration(array('ZNSource'))
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_english_title', 'story_etitle');
        $this->addFieldMapping('field_home_title', 'story_title');
        $this->addFieldMapping('field_oldid', 'story_id');
        $this->addFieldMapping('body', 'news_content');
        $this->addFieldMapping('body:summary', 'story_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        //$this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'story_date');
        $this->addFieldMapping('changed', 'LastUpdate');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('field_show_home_image')->defaultValue(0);
        $this->addFieldMapping('path','url');

        // $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'story_image');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znm.india.com/zn/marathi/');

      //  module_load_include('inc','pathauto','pathauto');

    }

    public function prepareRow($row) {

        if (strlen(trim($row->tag)) < 4 )
        {
            $row->tag = null;
        }
        if ($row->story_etitle == 'test')
        {
            $row->story_etitle = null;
        }
        if ($row->author_id==0){$row->author_id=null;}
        $row->news_content =  str_replace("<br>", "<br/>", $row->news_content);
        $row->news_content =  str_replace("<br/>", "\n", $row->news_content);
        $row->news_content =  str_replace("\n\n", "\n", $row->news_content);

            $row->url='marathi/'.$row->url;

        if (stripos('spacer.gif',$row->story_image) == false)
        {}
        else
        {
            $row->story_image = null;}

    }
    public function prepare($node, stdClass $row) {


        $keywords = explode(',', $row->tag);
        static $vid;
        if (!isset($vid)) {
            $vocabs = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'tags'));
            $vids = array_keys($vocabs);
            $vid = reset($vids);
        }
        foreach ($keywords as $index => $keyword) {

            $keyword=trim($keyword);
            if (!empty($keyword))
            {
                $matches = taxonomy_get_term_by_name($keyword);
                if (empty($matches)) {
                    $term = new stdClass;
                    $term->name = $keyword;
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
        //     if ($node->nid < 773001)
        {
            $html = str_get_html($node->body[LANGUAGE_NONE][0]['value']);

            $total_img = count($html->find("img"));
            if ($total_img > 0) {
                for ($i = 0; $i < $total_img; $i++) {
                    $element=$html->find("img", $i);
                    $src = $html->find("img", $i)->src;
                    if (stripos('spacer.gif',$src) == false)
                    {
                        $url = $src;
                        $url_info = parse_url($url);
                        $url_path_info = pathinfo($url_info['path']);

                        //This helps with filenames with spaces
                        $url = $url_info['scheme'] . '://' . $url_info['host']  . $url_path_info['dirname'] .'/'. rawurlencode($url_path_info['basename']);
                        $dest='public://' . str_replace(' ','_',$url_path_info['basename']);
                        //print $url;
                        $remoteDocPath = strtolower($url);
                        $doc = system_retrieve_file($remoteDocPath,$dest, true, FILE_EXISTS_RENAME);
                        //  $mfiles = drupal_add_existing_file($doc);
                        if ( strlen($doc->uri) > 10){
                            $html->find("img", $i)->src = str_replace('public://','/sites/default/files/', $doc->uri);
                        }
                        else{
                            $element->outertext ='';
                        }
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
 * The ZeeNews Marathi Authors Migration
 */
class ZNMAuthor extends ZeeNewsMigration {
    public function __construct() {

        parent::__construct();
        $this->description = t('Beer Drinkers of the world');
        $this->map = new MigrateSQLMap($this->machineName,
            array('author_id' => array(
                'type' => 'int',
                'not null' => TRUE,
                'description' => 'Account ID.'
            )
            ),
            MigrateDestinationUser::getKeySchema()
        );
        $query = Database::getConnection('zeemarathi101', 'for_migration')
            ->select('author_master','au')
            ->fields('au', array('author_id', 'author_name', 'author_email', 'googleid', 'author_thumb_image', 'author_twitter'));
        $query->condition('author_id', '6', '!=');
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
        $this->destination = new MigrateDestinationUser();


        // Our source table has two entries for 'alice', but we must have a unique
        // username in the Drupal 'users' table. dedupe() creates new, unique
        // destination values when the source field of that value already exists.
        // For example, if we're importing a user with name 'test' and a user
        // 'test' already exists in the target, we'll create a new user named
        // 'test_1'.
        // dedupe() takes the Drupal table and column for determining uniqueness.
        $this->addFieldMapping('name', 'author_name')
            ->dedupe('users', 'name');
     //   $this->addFieldMapping('uid', 'author_id');
        $this->addFieldMapping('status')
            ->defaultValue(1);
        $this->addFieldMapping('is_new')
            ->defaultValue(TRUE);
        $this->addFieldMapping('field_twitter', 'author_twitter');
        $this->addFieldMapping('field_google', 'googleid');

        $this->addFieldMapping('mail', 'author_email');
        $this->addFieldMapping('field_image', 'author_thumb_image');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znm.india.com/zn/marathi/');


        $this->addFieldMapping('pass')
            ->defaultValue('sdhjskdhskd@sdhjksds');

        $this->addFieldMapping('roles')
            ->defaultValue(DRUPAL_AUTHENTICATED_RID);


        if (module_exists('path')) {
            $this->addFieldMapping('path')
                ->issueGroup(t('DNM'));
            if (module_exists('pathauto')) {
                $this->addFieldMapping('pathauto')
                    ->issueGroup(t('DNM'));
            }
        }
    }
    public function prepareRow($row) {
        if ($row->author_twitter =='desk')        {$row->author_twitter = null;}

        if (strlen($row->googleid) < 20)        {$row->googleid =null;
        }
        if (stripos('spacer.gif',$row->author_image) == false)
        {}
        else        {            $row->author_image = null;            }
    }
}

/**
 * The Marathi Video CategoryMigration
 */
class ZNMVideoCategory extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');


        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'sectionid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );


        $query = Database::getConnection('zeemarathi101', 'for_migration')
            ->select('Video_Sections', 'cat'); //select distinct csection,cSection_Short from zn_vedio_upload
        $query->fields('cat', array('sectionid', 'sectionname'));
        $query->distinct();

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('video_sections');

        $this->addFieldMapping('name', 'sectionname');
        //$this->addFieldMapping('description', 'sectionid');


        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
        if (module_exists('path')) {
            $this->addFieldMapping('path')
                ->issueGroup(t('DNM'));
            if (module_exists('pathauto')) {
                $this->addFieldMapping('pathauto')
                    ->issueGroup(t('DNM'));
            }
        }
    }
    public function prepareRow($row) {

        $row->csection = utf8_encode(strip_tags(trim($row->csection)));


    }
}

/**
 * The Marathi Video Migration
 */
class ZNMVideo extends ZeeNewsMigration {
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

        $query = Database::getConnection('zeemarathi101', 'for_migration')
            ->select('news_videos', 'zvids'); //SELECT     TOP (100) PERCENT iVediouplaod, vFilename, dUploaddate, LTRIM(RTRIM(cSection)) AS csection, cDescription, cTitle, dbo.RemoveSpecialChars(cTitle) AS url         FROM         dbo.zn_vedio_upload  WHERE     (isYoutube = 0) ORDER BY iVediouplaod
        $query->fields('zvids', array('video_id','video_title','video_desc','video_file','video_image','sectionid','video_etitle','youtubecode', 'isYoutube','video_date','url'));

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
        $this->addFieldMapping('field_domain')->defaultValue("10");
        $this->addFieldMapping('field_isyoutube','isYoutube');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');

           $this->addFieldMapping('field_video_section', 'sectionid')
               ->sourceMigration('ZNMVideoCategory')
               ->arguments(array('source_type' => 'tid'));
        //   $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        // $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'video_image');
        // $this->addFieldMapping('field_image:source_dir')
        //    ->defaultValue('');

        //  $this->addFieldMapping('field_video_file:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
           $this->addFieldmapping('field_video_file:preserve_files')->defaultValue(TRUE);
         $this->addFieldMapping('field_video_file', 'video_file');
         $this->addFieldMapping('field_video_file:source_dir')
             ->defaultValue('http://origin.znm.india.com/zn/marathi/');

    }

    public function prepareRow($row) {
           if ($row->isYoutube == 1)
        {
               $row->vfilename = null;
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
            $row->video_image = 'http://origin.znm.india.com/zn/marathi/' . $row->video_image;

        }


        $row->video_title =  ((strip_tags(trim($row->video_title))));
        $row->video_desc = ((strip_tags(trim($row->video_desc))));



        $row->url='marathi/news/video' .$row->url;
    }
    public function prepare($node, stdClass $row) {
        // $node->nid=1505000 + $node->nid;

        // Let's pretend we couldn't have done this as easily in prepareRow...
        //  $node->path = 'news/' . $row->znm_news_id . '.html';
    }

}




/**
 * The Zee News Marathi Gallery Migration
 */
class ZNMGallery extends ZeeNewsMigration {
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

        $query = Database::getConnection('zeemarathi101', 'for_migration')
            ->select('news_gallery', 'znews');
        $query->fields('znews', array('photo_id','photo_title','url','photo_date','photo_image'));
        $query->condition('photo_id', '153544', '<=');
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
        $this->addFieldMapping('field_domain')->defaultValue("10");
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');
        $this->addFieldMapping('field_image', 'photo_image');

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
        if (strpos($row->photo_image,'http') !== false)
        {
            //print $row->img_path;
        }
        else
        {
            $row->photo_image = 'http://origin.znm.india.com/zn/marathi/' . $row->photo_image;

        }
        $row->url='marathi/news/images'.$row->url;
        //   $row->secid=1500000 + $row->secid;

        //   $row->znm_news_title= $row->znm_news_title .'Rana';

    }

}


/**
 * The NodeImagesMigration - Field Collection
 */
class ZNMGalleryImages extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $this->dependencies = array('ZNMGallery');
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


        $query = Database::getConnection('zeemarathi101', 'for_migration')
            ->select('news_gimages', 'znews');
        $query->fields('znews', array('image_id','image_title','image_file','photo_id'));
         $query->condition('photo_id', '153544', '<=');
//        $query->condition('photo_id', '3990', '<');

        $query->orderBy('image_id', 'asc');
//        $query->range(0, 200);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationFieldCollection('field_gallery_data',array('host_entity_type' => 'node'));

        $this->addFieldMapping('host_entity_id', 'photo_id')
            ->sourceMigration('ZNMGallery');
        $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
    //    $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'image_file');
        $this->addFieldMapping('field_caption', 'image_title');
        //   $this->addFieldMapping('pathauto')->defaultValue("0");
        //$this->addFieldMapping('field_caption')->defaultValue('Ultra Orthodox Jewish men wrapped in prayer shawls participate in the special "Blessing of the Sun" prayer at historic site of Masada overlooking the Dead Sea, in Masada, Israel, Wednesday. Devout Jews around the world on Wednesday observed a ritual perfo sdsds jsdhjsdhj 123');
        // ... Add field mapping here.


        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled

    }

    public function prepareRow($row) {
        if (strpos($row->image_file,'http') !== false)
        {
            //print $row->img_path;
        }
        else
        {
            $row->image_file = 'http://origin.znm.india.com/zn/marathi/' . $row->image_file;

        }
    }
}


/**
 * The Zee News Marathi Gallery Migration
 */
class ZNMSlideshow extends ZeeNewsMigration {
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

        $query = Database::getConnection('zeemarathi101', 'for_migration')
            ->select('slide_section', 'znews');
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
        $this->addFieldMapping('field_domain')->defaultValue("10");
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','status');
        $this->addFieldMapping('field_image', 'sec_img');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znm.india.com/zn/marathi/');

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
        module_load_include('inc','pathauto','pathauto');
    }

    public function prepareRow($row) {
        $row->status='marathi/slideshow/'. pathauto_cleanstring($row->sec_name).'_'.$row->sid.'.html';
    }

}



/**
 * The NodeImagesMigration - Field Collection
 */
class ZNMSlideImages extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $this->dependencies = array('ZNMSlideshow');
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

        $query = "SELECT nid,sid,story_image,story_title,story_desc FROM slidenews_master";
        $count_query = "SELECT COUNT(*) FROM slidenews_master";
//        $query = "SELECT story_id,story_title,sections,domain_id,sections,image_courtesy FROM news_master";
        $fields = array(
            'nid' => t('story_id'),
            'sid' => t('story_title ID'),
            'story_image' => t('Story Image'),
            'story_title' => t('Story Title'),
            'story_desc' => t('story desc'),
        );
        $this->source = new MigrateSourceMSSQL(array(
            'servername' => '180.179.93.213',
            'username' => 'sa',
            'password' => 'P@ssw0rd',
            'database' => 'ZeeNews_Marathi',
        ), $query, $count_query, $fields);



        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationFieldCollection('field_gallery_data',array('host_entity_type' => 'node'));

        $this->addFieldMapping('host_entity_id', 'sid')
            ->sourceMigration('ZNMSlideshow');
     //   $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        //    $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'story_image');
        $this->addFieldMapping('field_caption', 'story_title');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znm.india.com/zn/marathi/');

        //   $this->addFieldMapping('pathauto')->defaultValue("0");
        //$this->addFieldMapping('field_caption')->defaultValue('Ultra Orthodox Jewish men wrapped in prayer shawls participate in the special "Blessing of the Sun" prayer at historic site of Masada overlooking the Dead Sea, in Masada, Israel, Wednesday. Devout Jews around the world on Wednesday observed a ritual perfo sdsds jsdhjsdhj 123');
        // ... Add field mapping here.


        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled

    }

    public function prepareRow($row) {
            $row->story_title = '<h3>'.$row->story_title .'</h3><br/>' . $row->story_desc  ;

    }
}
