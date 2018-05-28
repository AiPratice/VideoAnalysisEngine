<template>
    <div>
        <Card>
            <p slot="title" class="card-title">
                <Icon type="android-checkbox-outline"></Icon>
                    标签搜索
            </p>
            <Card dis-hover>
                <p slot="title" class="card-title">物体标签</p>
                <Row>
                    <CheckboxGroup v-model="checkObjects">
                        <Col span="8" v-for="object in objectClasses" :key="object.objectID">
                            <Checkbox :label="object.object_class_id+''" v-if="object.cn_name != ''">{{object.cn_name}}</Checkbox>
                            <Checkbox :label="object.object_class_id+''" v-else>{{object.en_name}}</Checkbox>
                        </Col>
                    </CheckboxGroup>
                </Row>
            </Card>
            <br/>
            <Card>
                <p slot="title" class="card-title">人物</p>
                    <Row>
                    <CheckboxGroup v-model="checkPeople">
                        <Col span="8" v-for="person in persons" :key="person.PersonID">
                            <Checkbox :label="person.person_id+''">{{person.name}}</Checkbox>
                        </Col>
                    </CheckboxGroup>
                </Row>
            </Card>
            <br/>
            <Button type="primary" icon="ios-search" @click="handleSearch">检索</Button>
        </Card>
    </div>
</template>

<script>
import util from '../../../libs/util';
import config from '../../../config';

export default {
    name: 'tagSearch',
    mounted: function(){
        this.$store.dispatch('getClasses');
        this.$store.dispatch('getPersons');
    },
    computed: {
        objectClasses () {
            return this.$store.state.object.classes;
        },
        persons () {
            return this.$store.state.person.persons;
        },
    },
    data () {
        return {
            checkObjects:[],
            checkPeople:[]
        };
    },
    methods:{
        handleSearch() {
            console.log(this.checkObjects);
            console.log(this.checkPeople);
            this.$router.push({
                path: 'tag-search/results',
                name: 'tag-search-results',
                query: {
                    objects:JSON.stringify(this.checkObjects),
                    people:JSON.stringify(this.checkPeople)
                }
            });
        }
    }
};
</script>

