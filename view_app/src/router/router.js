import Main from '@/views/Main.vue';

// 不作为Main组件的子页面展示的页面单独写，如下
export const loginRouter = {
    path: '/login',
    name: 'login',
    meta: {
        title: 'Login - 登录'
    },
    component: resolve => { require(['@/views/login.vue'], resolve); }
};

export const page404 = {
    path: '/*',
    name: 'error-404',
    meta: {
        title: '404-页面不存在'
    },
    component: resolve => { require(['@/views/error-page/404.vue'], resolve); }
};

export const page403 = {
    path: '/403',
    meta: {
        title: '403-权限不足'
    },
    name: 'error-403',
    component: resolve => { require(['@//views/error-page/403.vue'], resolve); }
};

export const page500 = {
    path: '/500',
    meta: {
        title: '500-服务端错误'
    },
    name: 'error-500',
    component: resolve => { require(['@/views/error-page/500.vue'], resolve); }
};

/*export const preview = {
    path: '/preview',
    name: 'preview',
    component: resolve => { require(['@/views/form/article-publish/preview.vue'], resolve); }
};*/

export const locking = {
    path: '/locking',
    name: 'locking',
    component: resolve => { require(['@/views/main-components/lockscreen/components/locking-page.vue'], resolve); }
};

// 作为Main组件的子页面展示但是不在左侧菜单显示的路由写在otherRouter里
export const otherRouter = {
    path: '/',
    name: 'otherRouter',
    redirect: '/home',
    component: Main,
    children: [
        { path: 'home', title: {i18n: 'home'}, name: 'home_index', component: resolve => { require(['@/views/home/home.vue'], resolve); } },
        { path: 'ownspace', title: '个人中心', name: 'ownspace_index', component: resolve => { require(['@/views/own-space/own-space.vue'], resolve); } },
        { path: 'order/:order_id', title: '订单详情', name: 'order-info', component: resolve => { require(['@/views/advanced-router/component/order-info.vue'], resolve); } }, // 用于展示动态路由
        { path: 'shopping', title: '购物详情', name: 'shopping', component: resolve => { require(['@/views/advanced-router/component/shopping-info.vue'], resolve); } }, // 用于展示带参路由
        { path: 'shots/:video_id', title: '视频关键帧', name: 'shots_index', component: resolve => { require(['@/views/shots/shots.vue'], resolve); } },
        { path: '/video_objs/results/:video_id', title: '视频物体检测结果', name: 'video_objs_results', component:resolve => {require(['@/views/video-objs/video-objs-results.vue'], resolve); }},
        { path: '/video_faces/results/:video_id', title: '视频人脸识别结果', name: 'video_faces_results', component:resolve => {require(['@/views/video-faces/video-faces-results.vue'], resolve); }},
        { path: '/tag-search/results', title: '标签检索结果', name: 'tag-search-results', component: resolve => { require(['@/views/tag-search/tag-search-results.vue'], resolve); } }, // 标签检索结果
    ]
};

// 作为Main组件的子页面展示并且在左侧菜单显示的路由写在appRouter里
export const appRouter = [
    {
        path: '/search',
        icon: 'search',
        name: 'search',
        title: '视频检索',
        component: Main,
        children: [
            { path: 'index', title: '视频检索', name: 'search_index', component:resolve => {require(['@/views/search/search.vue'], resolve); }}
        ]
    },
    {
        path: '/videos',
        icon: 'film-marker',
        name: 'videos',
        title: '视频管理',
        component: Main,
        children: [
            { path: 'index', title: '视频管理', name: 'videos_index', component:resolve => {require(['@/views/videos/videos.vue'], resolve); }}
        ]
    },
    {
        path: '/persons',
        icon: 'man',
        name: 'persons',
        title: '预标注人物管理',
        component: Main,
        children: [
            { path: 'index', title: '预标注人物管理', name: 'persons_index', component:resolve => {require(['@/views/persons/persons.vue'], resolve); }}
        ]
    },
    {
        path: '/objectclasses',
        icon: 'pizza',
        name: 'objectclasses',
        title: '物体类型管理',
        component: Main,
        children: [
            { path: 'index', title: '物体类型管理', name: 'objectclasses_index', component:resolve => {require(['@/views/object-classes/object-classes.vue'], resolve); }}
        ]
    },
    {
        path: '/facetest',
        icon: 'social-octocat',
        name: 'facetest',
        title: '人脸识别测试',
        component: Main,
        children: [
            { path: 'index', title: '人脸识别测试', name: 'facetest_index', component:resolve => {require(['@/views/facetest/facetest.vue'], resolve); }}
        ]
    },
    {
        path: '/objecttest',
        icon: 'android-boat',
        name: 'objecttest',
        title: '物体检测测试',
        component: Main,
        children: [
            { path: 'index', title: '物体检测测试', name: 'objecttest_index', component:resolve => {require(['@/views/objecttest/objecttest.vue'], resolve); }}
        ]
    },
    {
        path: '/video_objs',
        icon: 'ionic',
        name: 'video_objs',
        title: '视频物体检测',
        component: Main,
        children: [
            { path: 'index', title: '视频物体检测', name: 'video_objs_index', component:resolve => {require(['@/views/video-objs/video-objs.vue'], resolve); }},
            
        ]
    },
    {
        path: '/video_faces',
        icon: 'outlet',
        name: 'video_faces',
        title: '视频人脸识别',
        component: Main,
        children: [
            { path: 'index', title: '视频人脸识别', name: 'video_faces_index', component:resolve => {require(['@/views/video-faces/video-faces.vue'], resolve); }},
            
        ]
    },
    /*{
        path: '/access',
        icon: 'key',
        name: 'access',
        title: '权限管理',
        component: Main,
        children: [
            { path: 'index', title: '权限管理', name: 'access_index', component: resolve => { require(['@/views/access/access.vue'], resolve); } }
        ]
    },
    {
        path: '/access-test',
        icon: 'lock-combination',
        title: '权限测试页',
        name: 'accesstest',
        access: 0,
        component: Main,
        children: [
            { path: 'index', title: '权限测试页', name: 'accesstest_index', access: 0, component: resolve => { require(['@/views/access/access-test.vue'], resolve); } }
        ]
    },*/
    // {
    //     path: '/charts',
    //     icon: 'ios-analytics',
    //     name: 'charts',
    //     title: '图表',
    //     component: Main,
    //     children: [
    //         { path: 'pie', title: '饼状图', name: 'pie', icon: 'ios-pie', component: resolve => { require('@/views/access/access.vue') },
    //         { path: 'histogram', title: '柱状图', name: 'histogram', icon: 'stats-bars', component: resolve => { require('@/views/access/access.vue') }

    //     ]
    // },
    /*{
        path: '/tables',
        icon: 'ios-grid-view',
        name: 'tables',
        title: '表格',
        component: Main,
        children: [
            { path: 'dragableTable', title: '可拖拽排序', name: 'dragable-table', icon: 'arrow-move', component: resolve => { require(['@/views/tables/dragable-table.vue'], resolve); } },
            { path: 'editableTable', title: '可编辑表格', name: 'editable-table', icon: 'edit', component: resolve => { require(['@/views/tables/editable-table.vue'], resolve); } },
            { path: 'searchableTable', title: '可搜索表格', name: 'searchable-table', icon: 'search', component: resolve => { require(['@/views/tables/searchable-table.vue'], resolve); } },
            { path: 'exportableTable', title: '表格导出数据', name: 'exportable-table', icon: 'code-download', component: resolve => { require(['@/views/tables/exportable-table.vue'], resolve); } },
            { path: 'table2image', title: '表格转图片', name: 'table-to-image', icon: 'images', component: resolve => { require(['@/views/tables/table-to-image.vue'], resolve); } }
        ]
    },
    {
        path: '/advanced-router',
        icon: 'ios-infinite',
        name: 'advanced-router',
        title: '高级路由',
        component: Main,
        children: [
            { path: 'mutative-router', title: '动态路由', name: 'mutative-router', icon: 'link', component: resolve => { require(['@/views/advanced-router/mutative-router.vue'], resolve); } },
            { path: 'argument-page', title: '带参页面', name: 'argument-page', icon: 'android-send', component: resolve => { require(['@/views/advanced-router/argument-page.vue'], resolve); } }
        ]
    },*/
    /*{
        path: '/error-page',
        icon: 'android-sad',
        title: '错误页面',
        name: 'errorpage',
        component: Main,
        children: [
            { path: 'index', title: '错误页面', name: 'errorpage_index', component: resolve => { require(['@/views/error-page/error-page.vue'], resolve); } }
        ]
    }*/
];

// 所有上面定义的路由都要写在下面的routers里
export const routers = [
    loginRouter,
    otherRouter,
    //preview,
    locking,
    ...appRouter,
    page500,
    page403,
    page404
];
